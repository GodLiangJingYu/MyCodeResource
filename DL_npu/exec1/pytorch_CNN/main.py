import torch
import torch.nn as nn
import torch.optim as optim
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader

# 1. 设置设备（这里使用CPU）
device = torch.device("cpu")
print(f"Using device: {device}")

# 2. 数据预处理和加载MNIST数据集
transform = transforms.Compose([
    transforms.ToTensor(),  # 将图像转换为Tensor
    transforms.Normalize((0.1307,), (0.3081,))  # MNIST数据集的均值和标准差
])

# 加载训练和测试数据集
train_dataset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
test_dataset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)

train_loader = DataLoader(dataset=train_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader(dataset=test_dataset, batch_size=64, shuffle=False)


# 3. 定义CNN模型
class SimpleCNN(nn.Module):
    def __init__(self):
        super(SimpleCNN, self).__init__()
        self.conv1 = nn.Conv2d(1, 32, kernel_size=3, padding=1)  # 输入1通道，输出32通道
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)  # 输入32通道，输出64通道
        self.pool = nn.MaxPool2d(kernel_size=2, stride=2)  # 最大池化
        self.fc1 = nn.Linear(64 * 7 * 7, 128)  # 全连接层
        self.fc2 = nn.Linear(128, 10)  # 输出10类（0-9数字）
        self.relu = nn.ReLU()
        self.dropout = nn.Dropout(0.5)  # Dropout防止过拟合

    def forward(self, x):
        x = self.pool(self.relu(self.conv1(x)))  # 卷积+激活+池化
        x = self.pool(self.relu(self.conv2(x)))  # 卷积+激活+池化
        x = x.view(-1, 64 * 7 * 7)  # 展平
        x = self.relu(self.fc1(x))  # 全连接层
        x = self.dropout(x)  # Dropout
        x = self.fc2(x)  # 输出层
        return x


# 4. 初始化模型、损失函数和优化器
model = SimpleCNN().to(device)
criterion = nn.CrossEntropyLoss()  # 交叉熵损失
optimizer = optim.Adam(model.parameters(), lr=0.001)  # Adam优化器


# 5. 训练模型
def train(model, train_loader, criterion, optimizer, epochs=5):
    model.train()
    for epoch in range(epochs):
        running_loss = 0.0
        for i, (images, labels) in enumerate(train_loader):
            images, labels = images.to(device), labels.to(device)

            # 前向传播
            outputs = model(images)
            loss = criterion(outputs, labels)

            # 反向传播和优化
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            running_loss += loss.item()
            if (i + 1) % 100 == 0:
                print(f'Epoch [{epoch + 1}/{epochs}], Step [{i + 1}], Loss: {running_loss / 100:.4f}')
                running_loss = 0.0


# 6. 测试模型
def test(model, test_loader):
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():
        for images, labels in test_loader:
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    accuracy = 100 * correct / total
    print(f'Test Accuracy: {accuracy:.2f}%')


# 7. 运行训练和测试
if __name__ == "__main__":
    print("Starting training...")
    train(model, train_loader, criterion, optimizer, epochs=5)
    print("Training finished. Starting testing...")
    test(model, test_loader)