import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.metrics import r2_score
import scipy.stats as stats

np.random.seed(42)
n_samples = 700
# 模拟 GDP 数据 (X)，假设范围在 1000 到 10000 之间
X = np.random.uniform(1000, 10000, n_samples).reshape(-1, 1)
# 模拟 居民消费水平 (Y)，假设模型 Y = 0.6 * X + 500 + 噪声
# β1 = 0.6 (斜率), β0 = 500 (截距)
noise = np.random.normal(0, 500, n_samples).reshape(-1, 1) # 增加一些随机噪声
Y = 0.6 * X + 500 + noise

print("数据准备完成。样本数量: {}, 特征数量: 1".format(n_samples))

# 2. 线性模型建立与估计 (Model Building & Estimation)
# 使用 sklearn 的 LinearRegression 进行最小二乘法估计
model = LinearRegression()
model.fit(X, Y)

beta1_est = model.coef_[0][0]
beta0_est = model.intercept_[0]
y_pred = model.predict(X)

print("\n--- 模型参数估计 ---")
print(f"截距 (beta0): {beta0_est:.4f}")
print(f"斜率 (beta1): {beta1_est:.4f}")
print(f"拟合方程: Y = {beta0_est:.4f} + {beta1_est:.4f} * X")

# 3. 模型检验 (Model Testing)
# 由于 cnn_env 环境中可能没有 statsmodels，我们使用 numpy 和 scipy 手动计算统计量

# (1) R^2 检验
r2 = r2_score(Y, y_pred)

# (2) 计算统计量 (t检验 和 F检验)
# 自由度
df_total = n_samples - 1
df_model = 1                # 预测变量个数
df_resid = n_samples - df_model - 1 # 残差自由度

# 均方误差 (MSE) 和 总平方和 (SST), 回归平方和 (SSR), 残差平方和 (SSE)
sse = np.sum((Y - y_pred) ** 2)
sst = np.sum((Y - np.mean(Y)) ** 2)
ssr = sst - sse
mse = sse / df_resid

# F 统计量: MSR / MSE
msr = ssr / df_model
F_stat = msr / mse
F_p_value = 1 - stats.f.cdf(F_stat, df_model, df_resid)

# t 统计量
# 首先计算参数的标准误差
# var(beta1) = MSE / sum((x_i - mean(x))^2)
x_mean = np.mean(X)
sum_sq_diff_x = np.sum((X - x_mean) ** 2)
se_beta1 = np.sqrt(mse / sum_sq_diff_x)

# var(beta0) = MSE * (1/n + mean(x)^2 / sum((x_i - mean(x))^2))
se_beta0 = np.sqrt(mse * (1/n_samples + (x_mean**2) / sum_sq_diff_x))

t_stat_beta1 = beta1_est / se_beta1
t_stat_beta0 = beta0_est / se_beta0

# 双尾检验 P 值
p_val_beta1 = 2 * (1 - stats.t.cdf(np.abs(t_stat_beta1), df_resid))
p_val_beta0 = 2 * (1 - stats.t.cdf(np.abs(t_stat_beta0), df_resid))

print("\n--- 模型检验结果 ---")
print(f"R^2 (拟合优度): {r2:.4f}")
print(f"\nF 检验 (整体显著性):")
print(f"  F-statistic: {F_stat:.4f}")
print(f"  P-value: {F_p_value:.4e}")

print(f"\nt 检验 (参数显著性):")
print(f"  beta1 (斜率) -> t-stat: {t_stat_beta1:.4f}, p-value: {p_val_beta1:.4e}")
print(f"  beta0 (截距) -> t-stat: {t_stat_beta0:.4f}, p-value: {p_val_beta0:.4e}")

# 4. 结果可视化 (Visualization)
plt.figure(figsize=(10, 6))
plt.scatter(X, Y, color='blue', alpha=0.5, label='Actual Data')
plt.plot(X, y_pred, color='red', linewidth=2, label='Fitted Line')
plt.title('Linear Regression: GDP vs Consumption (Simulated)')
plt.xlabel('GDP')
plt.ylabel('Consumption Level')
plt.legend()
plt.grid(True)
output_img = 'linear_model_experiment_result.png'
plt.savefig(output_img)
print(f"\n可视化结果已保存为: {output_img}")

# 5. 结果分析 (Analysis - Auto Generated Text)
print("\n--- 结果分析 ---")
print("1. 模型拟合效果:")
if r2 > 0.8:
    print(f"   R^2值为 {r2:.2f}，显示模型拟合效果非常好，自变量 GDP 能很好地解释居民消费水平的变化。")
elif r2 > 0.5:
    print(f"   R^2值为 {r2:.2f}，模型拟合效果良好。")
else:
    print(f"   R^2值为 {r2:.2f}，模型拟合效果一般。")

print("2. 参数显著性:")
if p_val_beta1 < 0.05:
    print(f"   GDP (X) 的系数 P 值 < 0.05，表明 GDP 对居民消费水平有显著影响。")
else:
    print(f"   GDP (X) 的系数 P 值 >= 0.05，表明 GDP 对居民消费水平影响不显著。")

if F_p_value < 0.05:
    print("3. 模型整体显著性:")
    print("   F 检验 P 值 < 0.05，模型整体显著。")
