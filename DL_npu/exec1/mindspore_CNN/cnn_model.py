import mindspore as ms
import mindspore.nn as nn
import mindspore.ops as ops
from mindspore import context
import numpy as np
import os

# 设置运行模式
context.set_context(mode=context.GRAPH_MODE, device_target="CPU")


# 定义CNN模型
class LeNet5(nn.Cell):
    def __init__(self, num_classes=10):
        super(LeNet5, self).__init__()
        # 卷积层
        self.conv1 = nn.Conv2d(1, 6, kernel_size=5, pad_mode="valid")
        self.conv2 = nn.Conv2d(6, 16, kernel_size=5, pad_mode="valid")
        # 全连接层
        self.fc1 = nn.Dense(16 * 4 * 4, 120)  # 注意: 输入是28x28, 经过卷积和池化后变为4x4
        self.fc2 = nn.Dense(120, 84)
        self.fc3 = nn.Dense(84, num_classes)
        # 激活函数
        self.relu = nn.ReLU()
        # 池化层
        self.max_pool2d = nn.MaxPool2d(kernel_size=2, stride=2)
        # Flatten层
        self.flatten = nn.Flatten()

    def construct(self, x):
        # 输入: [batch_size, 1, 28, 28]
        x = self.conv1(x)  # -> [batch_size, 6, 24, 24]
        x = self.relu(x)
        x = self.max_pool2d(x)  # -> [batch_size, 6, 12, 12]
        x = self.conv2(x)  # -> [batch_size, 16, 8, 8]
        x = self.relu(x)
        x = self.max_pool2d(x)  # -> [batch_size, 16, 4, 4]
        x = self.flatten(x)  # -> [batch_size, 16*4*4]
        x = self.fc1(x)  # -> [batch_size, 120]
        x = self.relu(x)
        x = self.fc2(x)  # -> [batch_size, 84]
        x = self.relu(x)
        x = self.fc3(x)  # -> [batch_size, 10]
        return x


# 数据加载函数，使用.npy文件
def create_dataset_from_numpy(data_dir, batch_size=32):
    # 加载.npy文件
    images_path = os.path.join(data_dir, 'train_images.npy')
    labels_path = os.path.join(data_dir, 'train_labels.npy')

    if not os.path.exists(images_path) or not os.path.exists(labels_path):
        raise ValueError(f"Data files not found at {data_dir}. Make sure train_images.npy and train_labels.npy exist.")

    images = np.load(images_path)
    labels = np.load(labels_path)

    print(f"加载了 {len(images)} 个训练样本")
    print(f"图像形状: {images.shape}, 标签形状: {labels.shape}")

    # 预处理图像
    # 调整形状：[样本数, 高, 宽] -> [样本数, 1, 高, 宽]
    images = images.reshape(-1, 1, 28, 28).astype(np.float32) / 255.0

    # 创建数据集
    data = []
    for i in range(0, len(images), batch_size):
        end = min(i + batch_size, len(images))
        batch_images = ms.Tensor(images[i:end])
        batch_labels = ms.Tensor(labels[i:end], ms.int32)
        data.append((batch_images, batch_labels))

    return data


# 训练模型
def train_model():
    # 创建模型实例
    net = LeNet5()

    # 定义损失函数和优化器
    loss_fn = nn.CrossEntropyLoss()
    optimizer = nn.Momentum(net.trainable_params(), learning_rate=0.01, momentum=0.9)

    # 定义训练网络
    model = ms.Model(net, loss_fn=loss_fn, optimizer=optimizer)

    # 加载数据集
    dataset_path = "./MNIST_Data"  # 包含.npy文件的目录
    try:
        data = create_dataset_from_numpy(dataset_path)
    except ValueError as e:
        print(f"Error loading dataset: {e}")
        return

    # 训练模型
    print("Starting training...")
    epochs = 5
    for epoch in range(epochs):
        epoch_loss = 0
        for step, (images, labels) in enumerate(data):
            def forward_fn(data, label):
                logits = net(data)
                loss = loss_fn(logits, label)
                return loss, logits

            # 计算梯度并更新权重
            grad_fn = ms.value_and_grad(forward_fn, None, optimizer.parameters, has_aux=True)
            (loss, _), grads = grad_fn(images, labels)
            loss = ms.ops.depend(loss, optimizer(grads))
            epoch_loss += loss.asnumpy()

            if step % 50 == 0:
                print(f"Epoch: {epoch + 1}/{epochs}, Step: {step}, Loss: {loss.asnumpy():.6f}")

        print(f"Epoch: {epoch + 1}/{epochs}, Average Loss: {epoch_loss / len(data):.6f}")

    print("Training complete!")

    # 保存模型
    ms.save_checkpoint(net, "lenet5.ckpt")
    print("Model saved!")


if __name__ == "__main__":
    train_model()