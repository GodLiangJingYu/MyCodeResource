

import os
import time
import gzip
import numpy as np
from tqdm import tqdm

import mindspore as ms
import mindspore.nn as nn
import mindspore.dataset as ds
import mindspore.dataset.vision as vision
import mindspore.dataset.transforms as transforms
from mindspore import Tensor, context, Model
from mindspore.train.callback import LossMonitor, TimeMonitor
from mindspore.nn import Accuracy
from mindspore import Callback

class MyPrintCallback(Callback):
    def step_end(self, run_context):
        cb_params = run_context.original_args()
        print(f"Step: {cb_params.cur_step_num}, Loss: {cb_params.net_outputs}")

context.set_context(mode=context.GRAPH_MODE, device_target="CPU")

DATA_ROOT = r"D:\MyCodeResource\DL_npu\exec1\data\MNIST\raw"
BATCH_SIZE = 128
NUM_EPOCHS = 10
LEARNING_RATE = 1e-3
SEED = 2025
ms.set_seed(SEED)

def _load_gz(path):
    """读取 .gz 文件并返回 numpy 数组"""
    with gzip.open(path, 'rb') as f:
        data = np.frombuffer(f.read(), dtype=np.uint8, offset=16 if 'image' in path else 8)
    if 'image' in path:
        data = data.reshape(-1, 28, 28)
    return data

class MNISTDataset:
    def __init__(self, root, train=True):
        self.root = root
        self.train = train
        prefix = "train" if train else "t10k"
        self.images = _load_gz(os.path.join(root, f"{prefix}-images-idx3-ubyte.gz"))
        self.labels = _load_gz(os.path.join(root, f"{prefix}-labels-idx1-ubyte.gz"))

    def __getitem__(self, idx):
        img = self.images[idx].astype(np.float32) / 255.0   # 归一化到 [0,1]
        label = int(self.labels[idx])
        return img, label

    def __len__(self):
        return len(self.labels)

def create_dataset(root, train=True, batch_size=128, num_parallel_workers=4):
    ds_raw = MNISTDataset(root, train=train)
    ms_dataset = ds.GeneratorDataset(ds_raw, ["image", "label"],
                                     shuffle=train,
                                     num_parallel_workers=num_parallel_workers)

    # 数据增强（仅训练集）
    if train:
        trans = [
            vision.Rescale(1.0 / 255.0, 0.0),   # 已经在 __getitem__ 做了，这里保持兼容
            vision.HWC2CHW()
        ]
    else:
        trans = [vision.HWC2CHW()]

    ms_dataset = ms_dataset.map(operations=trans,
                                input_columns=["image"],
                                num_parallel_workers=num_parallel_workers)
    ms_dataset = ms_dataset.batch(batch_size, drop_remainder=True)
    return ms_dataset

class LeNet5(nn.Cell):
    def __init__(self, num_class=10):
        super().__init__()
        self.conv1 = nn.Conv2d(1, 6, 5, pad_mode='valid')
        self.conv2 = nn.Conv2d(6, 16, 5, pad_mode='valid')
        self.fc1   = nn.Dense(16 * 4 * 4, 120)
        self.fc2   = nn.Dense(120, 84)
        self.fc3   = nn.Dense(84, num_class)
        self.relu  = nn.ReLU()
        self.max_pool2d = nn.MaxPool2d(kernel_size=2, stride=2)
        self.flatten = nn.Flatten()

    def construct(self, x):
        x = x.expand_dims(1)               # (N,28,28) -> (N,1,28,28)
        x = self.relu(self.conv1(x))
        x = self.max_pool2d(x)
        x = self.relu(self.conv2(x))
        x = self.max_pool2d(x)
        x = self.flatten(x)
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.fc3(x)
        return x

net = LeNet5()
loss_fn = nn.SoftmaxCrossEntropyWithLogits(sparse=True, reduction='mean')
optim = nn.Adam(net.trainable_params(), learning_rate=LEARNING_RATE)

class EpochSummary(Callback):
    def __init__(self):
        self.epoch_start_time = None
        self.total_train_time = 0.0
        self.epoch_losses = []

    def on_train_epoch_begin(self, run_context):
        self.epoch_start_time = time.time()

    def on_train_epoch_end(self, run_context):
        cb_params = run_context.original_args()
        epoch = cb_params.cur_epoch_num
        loss = cb_params.net_outputs   # 已经是标量 Tensor
        loss_val = loss.asnumpy().item()
        self.epoch_losses.append(loss_val)

        epoch_time = time.time() - self.epoch_start_time
        self.total_train_time += epoch_time

        print(f"\n=== Epoch {epoch} finished ===")
        print(f"  Loss      : {loss_val:.5f}")
        print(f"  Time      : {epoch_time:.2f}s")

    def on_train_end(self, run_context):
        avg_loss = np.mean(self.epoch_losses)
        print("\n" + "="*50)
        print("【训练结束】")
        print(f"  总训练时间   : {self.total_train_time:.2f}s")
        print(f"  平均每epoch损失 : {avg_loss:.5f}")
        print("="*50)

summary_cb = EpochSummary()


train_dataset = create_dataset(DATA_ROOT, train=True,  batch_size=BATCH_SIZE)
test_dataset  = create_dataset(DATA_ROOT, train=False, batch_size=BATCH_SIZE)


model = Model(net, loss_fn=loss_fn, optimizer=optim, metrics={"Accuracy": Accuracy()})

print("开始训练...")
model.train(NUM_EPOCHS,
            train_dataset,
            callbacks=[LossMonitor(per_print_times=train_dataset.get_dataset_size()//2),
                       TimeMonitor(),
                       summary_cb],
            dataset_sink_mode=False)


print("\n开始评估...")
acc = model.eval(test_dataset, dataset_sink_mode=False)
test_acc = acc["Accuracy"]

print("\n" + "="*50)
print("【最终评估结果】")
print(f"  测试集准确率 : {test_acc*100:5.2f}%")
print(f"  训练总耗时   : {summary_cb.total_train_time:.2f}s")
print(f"  平均损失     : {np.mean(summary_cb.epoch_losses):.5f}")
print("="*50)