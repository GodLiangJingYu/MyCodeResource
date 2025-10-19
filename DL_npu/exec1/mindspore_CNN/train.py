import mindspore
from mindspore import nn, dataset as ds, Model
from mindspore.nn import Adam
from mindspore.dataset.transforms import Compose, transforms
from mindspore.dataset.vision import ToType, Normalize, Resize, HWC2CHW

# 步骤1: 定义CNN模型（LeNet-5变体）
class LeNet5(nn.Cell):
    def __init__(self, num_class=10):
        super(LeNet5, self).__init__()
        self.conv1 = nn.Conv2d(1, 6, 5, pad_mode='valid')
        self.conv2 = nn.Conv2d(6, 16, 5, pad_mode='valid')
        self.fc1 = nn.Dense(16 * 4 * 4, 120)
        self.fc2 = nn.Dense(120, 84)
        self.fc3 = nn.Dense(84, num_class)
        self.max_pool2d = nn.MaxPool2d(kernel_size=2, stride=2)
        self.relu = nn.ReLU()
        self.flatten = nn.Flatten()

    def construct(self, x):
        x = self.relu(self.conv1(x))
        x = self.max_pool2d(x)
        x = self.relu(self.conv2(x))
        x = self.max_pool2d(x)
        x = self.flatten(x)
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.fc3(x)
        return x

# 步骤2: 加载MNIST数据集
def create_dataset(batch_size=32):
    # 下载MNIST数据集（MindSpore会自动下载到~/.mindspore下）
    train_dataset = ds.MnistDataset(dataset_dir='./MNIST_Data/train', shuffle=True)
    test_dataset = ds.MnistDataset(dataset_dir='./MNIST_Data/test', shuffle=False)

    # 数据预处理
    trans = Compose([
        Resize(28),  # 调整大小
        Normalize((0.1307,), (0.3081,)),  # 归一化
        HWC2CHW(),  # 通道转换
        ToType(mindspore.float32)
    ])

    train_dataset = train_dataset.map(trans, 'image').batch(batch_size)
    test_dataset = test_dataset.map(trans, 'image').batch(batch_size)

    return train_dataset, test_dataset

# 步骤3: 训练和评估
if __name__ == "__main__":
    mindspore.set_context(mode=mindspore.GRAPH_MODE)  # 图模式（或PYNATIVE_MODE调试）

    net = LeNet5()
    loss = nn.SoftmaxCrossEntropyWithLogits(sparse=True, reduction='mean')
    optimizer = Adam(net.trainable_params(), learning_rate=0.001)

    model = Model(net, loss_fn=loss, optimizer=optimizer, metrics={'accuracy': nn.Accuracy()})

    train_dataset, test_dataset = create_dataset()

    # 训练
    model.train(epoch=5, train_dataset=train_dataset, callbacks=None)

    # 评估
    acc = model.eval(test_dataset)
    print("Accuracy:", acc)