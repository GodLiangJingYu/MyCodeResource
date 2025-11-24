import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import numpy as np
import os
import time
import matplotlib.pyplot as plt

# 设置随机种子以确保结果可复现
tf.random.set_seed(42)
np.random.seed(42)

# 定义数据路径
data_path = r'D:\MyCodeResource\DL_npu\exec1\data\MNIST\raw'

# 1. 数据加载与预处理
# TensorFlow内置了MNIST数据集，虽然路径可能不同，但我们可以直接加载并模拟从指定路径加载的过程
# 如果你有从指定路径加载自定义MNIST格式数据的需求，请提供具体文件格式（如npy, png等）
# 这里我们使用内置的MNIST数据集，并假设它代表了你路径下的数据
print("正在加载MNIST数据集...")
(x_train, y_train), (x_test, y_test) = keras.datasets.mnist.load_data()

# 数据预处理
# 调整维度 (28, 28) -> (28, 28, 1) 以适应CNN输入
x_train = x_train.reshape(x_train.shape[0], 28, 28, 1).astype('float32')
x_test = x_test.reshape(x_test.shape[0], 28, 28, 1).astype('float32')

# 归一化像素值到 [0, 1] 范围
x_train = x_train / 255.0
x_test = x_test / 255.0

# 将标签转换为one-hot编码（可选，取决于损失函数）
# y_train = keras.utils.to_categorical(y_train, 10)
# y_test = keras.utils.to_categorical(y_test, 10)

print(f"训练数据形状: {x_train.shape}")
print(f"训练标签形状: {y_train.shape}")
print(f"测试数据形状: {x_test.shape}")
print(f"测试标签形状: {y_test.shape}")

# 2. 构建CNN模型
def create_cnn_model():
    model = keras.Sequential([
        layers.Conv2D(32, (3, 3), activation='relu', input_shape=(28, 28, 1)),
        layers.MaxPooling2D((2, 2)),
        layers.Conv2D(64, (3, 3), activation='relu'),
        layers.MaxPooling2D((2, 2)),
        layers.Conv2D(64, (3, 3), activation='relu'),
        layers.Flatten(),
        layers.Dense(64, activation='relu'),
        layers.Dense(10, activation='softmax') # 10个类别，使用softmax
    ])
    return model

model = create_cnn_model()

# 查看模型结构
print("\n模型结构:")
model.summary()

# 3. 编译模型
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy', # 因为标签是整数而非one-hot
              metrics=['accuracy'])

# 4. 设置训练参数
EPOCHS = 10
BATCH_SIZE = 128

# 5. 开始训练并计时
print("\n开始训练模型...")
start_time = time.time()

# 使用Keras内置的训练回调来显示进度和记录历史
history = model.fit(x_train, y_train,
                    epochs=EPOCHS,
                    batch_size=BATCH_SIZE,
                    validation_data=(x_test, y_test),
                    verbose=1) # verbose=1 显示进度条

end_time = time.time()
training_time = end_time - start_time

# 6. 评估模型
print("\n正在评估模型在测试集上的表现...")
test_loss, test_acc = model.evaluate(x_test, y_test, verbose=0)
print(f"\n--- 训练总结 ---")
print(f"模型在测试集上的准确率: {test_acc:.4f}")
print(f"模型在测试集上的损失: {test_loss:.4f}")
print(f"总训练时间: {training_time:.2f} 秒")

# 7. 显示训练过程中的准确率和损失曲线
# 假设训练历史已保存在 'history' 变量中
acc = history.history['accuracy']
val_acc = history.history['val_accuracy']
loss = history.history['loss']
val_loss = history.history['val_loss']
epochs_range = range(EPOCHS)

plt.figure(figsize=(12, 4))
plt.subplot(1, 2, 1)
plt.plot(epochs_range, acc, label='训练准确率')
plt.plot(epochs_range, val_acc, label='验证准确率')
plt.legend(loc='lower right')
plt.title('训练和验证准确率')

plt.subplot(1, 2, 2)
plt.plot(epochs_range, loss, label='训练损失')
plt.plot(epochs_range, val_loss, label='验证损失')
plt.legend(loc='upper right')
plt.title('训练和验证损失')
plt.show()

print("\n训练完成！")