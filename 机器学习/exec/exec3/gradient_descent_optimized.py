import numpy as np
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt

# 数据准备
def prepare_data():
    try:
        from sklearn.datasets import load_boston
        data = load_boston()
    except ImportError:
        from sklearn.datasets import fetch_openml
        data = fetch_openml(data_id=531, as_frame=False, parser='auto')
    
    x = data.data.astype(float)
    y = data.target.astype(float)
    return x, y

# MSE损失函数
def mse_loss(y_true, y_pred):
    return np.mean((y_true - y_pred) ** 2)

# 梯度计算
def compute_gradients(x, y_true, y_pred):
    n = len(x)
    dw = np.dot(x.T, y_pred - y_true) / n
    db = np.mean(y_pred - y_true)
    return dw, db

# 梯度下降训练
def gradient_descent(x, y, learning_rate=0.001, epochs=1000):
    # 初始化参数
    W = np.zeros(x.shape[1])
    b = np.zeros(1)
    loss_history = []
    
    # 训练循环
    for epoch in range(epochs + 1):
        # 前向传播
        y_pred = np.dot(x, W) + b
        
        # 计算损失
        loss = mse_loss(y, y_pred)
        loss_history.append(loss)
        
        # 计算梯度
        dw, db = compute_gradients(x, y, y_pred)
        
        # 参数更新
        W -= learning_rate * dw
        b -= learning_rate * db
        
        # 打印进度
        if epoch % 100 == 0:
            print(f'epoch {epoch}, loss {loss:.4f}')
    
    return W, b, loss_history

# 可视化损失变化
def plot_loss(loss_history):
    plt.figure(figsize=(10, 6))
    plt.plot(loss_history)
    plt.title('Training Loss Over Time')
    plt.xlabel('Epoch')
    plt.ylabel('MSE Loss')
    plt.grid(True)
    plt.savefig('gradient_descent_loss.png', dpi=300, bbox_inches='tight')
    print("损失函数图表已保存为: gradient_descent_loss.png")

# 主函数
def main():
    print("梯度下降法实验 - 波士顿房价预测")
    print("=" * 40)
    
    # 1. 准备数据
    x, y = prepare_data()
    
    # 2. 数据标准化
    scaler = StandardScaler()
    x_scaled = scaler.fit_transform(x)
    
    # 3. 梯度下降训练
    print("\n开始训练...")
    W, b, loss_history = gradient_descent(x_scaled, y, learning_rate=0.01, epochs=1000)
    
    # 4. 模型测试
    evaluate_model(scaler, W, b)
    
    # 5. 可视化训练过程
    plot_loss(loss_history)

# 模型测试
def evaluate_model(scaler, W, b):
    print("\n模型测试:")
    print("-" * 20)
    
    # 使用PDF中的测试数据
    x_test_raw = np.array([[0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3]])
    x_test = scaler.transform(x_test_raw)
    y_pred = np.dot(x_test, W) + b
    
    print(f"测试输入: {x_test_raw[0]}")
    print(f"预测房价: {y_pred[0]:.2f}")

if __name__ == "__main__":
    main()