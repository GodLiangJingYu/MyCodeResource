import mindspore
from mindspore import nn, Model, context
from mindspore.dataset import vision, transforms
from mindspore.dataset import Cifar10Dataset
from mindspore.nn import Accuracy
import mindspore.ops as ops
import os

# 1. 设置运行环境
context.set_context(mode=context.GRAPH_MODE, device_target="CPU")

# 2. 定义CNN网络（LeNet5）
class LeNet5(nn.Cell):
    def __init__(self, num_class=10):
        super(LeNet5, self).__init__()
        self.conv1 = nn.Conv2d(3, 6, 5, pad_mode='valid')
        self.conv2 = nn.Conv2d(6, 16, 5, pad_mode='valid')
        self.fc1 = nn.Dense(16 * 5 * 5, 120) # (32-5+1)=28 -> (28/2=14) -> (14-5+1)=10 -> (10/2=5) -> 16*5*5=400
        self.fc2 = nn.Dense(120, 84)
        self.fc3 = nn.Dense(84, num_class)
        self.relu = nn.ReLU()
        self.max_pool2d = nn.MaxPool2d(kernel_size=2, stride=2)
        self.flatten = nn.Flatten()

    def construct(self, x):
        x = self.conv1(x)
        x = self.relu(x)
        x = self.max_pool2d(x)
        x = self.conv2(x)
        x = self.relu(x)
        x = self.max_pool2d(x)
        x = self.flatten(x)
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        x = self.relu(x)
        x = self.fc3(x)
        return x

# 3. 数据预处理
def create_dataset(data_path, batch_size=32, shuffle=True, usage='train'): # 添加 usage 参数
    # 定义数据集
    data_set = Cifar10Dataset(dataset_dir=data_path, usage=usage, shuffle=shuffle)

    # 定义图像变换
    image_transforms = [
        vision.Rescale(1.0 / 255.0, 0),
        vision.Normalize(mean=[0.4914, 0.4822, 0.4465], std=[0.2023, 0.1994, 0.2010]),
        vision.HWC2CHW()
    ]

    # 定义标签变换
    label_transform = transforms.TypeCast(mindspore.int32)

    # 应用变换
    data_set = data_set.map(image_transforms, input_columns="image")
    data_set = data_set.map(label_transform, input_columns="label")

    # 批处理
    data_set = data_set.batch(batch_size, drop_remainder=True)

    return data_set

# 4. 加载训练数据集
data_path = r"C:\Users\86180\Desktop\MyCodeResource\DL_npu\exec1\mindspore_CNN\cifar-10-batches-bin"
if not os.path.exists(data_path):
    print(f"错误：找不到数据集路径 {data_path}")
    print("请下载并解压CIFAR-10数据集到指定路径。")
    exit()

train_dataset = create_dataset(data_path, batch_size=32, usage='train')
# 为了计算准确率，还需要加载测试数据集
test_dataset = create_dataset(data_path, batch_size=32, usage='test', shuffle=False) # 测试集通常不打乱

# 5. 实例化网络
network = LeNet5(num_class=10)

# 6. 定义损失函数和优化器
net_loss = nn.SoftmaxCrossEntropyWithLogits(sparse=True, reduction='mean')
net_opt = nn.Momentum(network.trainable_params(), learning_rate=0.01, momentum=0.9)

# 7. 配置模型
model = Model(network, net_loss, net_opt, metrics={'Accuracy': Accuracy()})

# 8. 训练模型
print("开始训练...")
epoch_num = 10

# 定义回调函数列表，包括 LossMonitor 和 ModelCheckpoint (可选)
# LossMonitor 会自动打印每个 epoch 的平均损失
from mindspore.train.callback import LossMonitor, ModelCheckpoint, CheckpointConfig, TimeMonitor
# LossMonitor 通常需要传入训练数据集的大小
dataset_size = train_dataset.get_dataset_size()
print(f"数据集大小 (steps per epoch): {dataset_size}")

# 创建 TimeMonitor 来显示每个 epoch 的耗时
time_cb = TimeMonitor(data_size=dataset_size)
# LossMonitor 显示损失
loss_cb = LossMonitor(per_print_times=dataset_size) # 每个 epoch 打印一次平均损失

# 使用 callbacks 列表
callbacks = [time_cb, loss_cb]

model.train(epoch_num, train_dataset, callbacks=callbacks, dataset_sink_mode=False) # CPU模式下建议设置 dataset_sink_mode=False

print("训练完成.")

# 9. 评估模型 (可选，但有助于了解训练效果)
print("开始评估模型...")
# 使用测试集评估
eval_metrics = model.eval(test_dataset, dataset_sink_mode=False) # CPU模式下建议设置 dataset_sink_mode=False
print(f"评估结果: {eval_metrics}")

# 10. 保存模型（可选）
# mindspore.save_checkpoint(network, "lenet5_cifar10.ckpt")
# print("模型已保存为 lenet5_cifar10.ckpt")