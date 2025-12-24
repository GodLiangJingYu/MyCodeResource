import numpy as np
import math
import statsmodels.api as sm
import matplotlib.pyplot as plt


# 1. Manual Implementation (based on the PDF "Program Code Reference")
class SimpleLinear:
    def __init__(self, w_true, b_true, train_num):
        self.w_true = w_true
        self.b_true = b_true
        self.train_num = train_num
        self.makeData()
        self.x_mean = np.mean(self.x_train)

    def makeData(self):
        # Generate synthetic data simulating GDP (x) and Consumption (y)
        # Using positive values for GDP to be realistic
        np.random.seed(42)
        self.x_train = np.random.rand(self.train_num) * 10  # GDP range 0-10
        # Linear relation + noise
        self.y_train = self.w_true * self.x_train + self.b_true + np.random.randn(self.train_num) * 1.5

    def compute(self):
        up, below_1, below_2 = 0, 0, 0

        # Calculate slope w
        # Formula: sum(y * (x - x_mean)) / sum((x - x_mean)^2)
        # The PDF implementation calculates denominator as: sum(x^2) - (1/n)*(sum(x))^2
        # which is equivalent to sum((x - x_mean)^2)

        for x_i, y_i in zip(self.x_train, self.y_train):
            up += y_i * (x_i - self.x_mean)
            below_1 += x_i * x_i
            below_2 += x_i

        w = up / (below_1 - (1 / self.train_num) * below_2 * below_2)

        # Calculate intercept b
        # Formula: mean(y) - w * mean(x)
        # PDF implementation sums (y_i - w*x_i) then divides by n
        b_com = 0
        for x_i, y_i in zip(self.x_train, self.y_train):
            b_com += (y_i - w * x_i)

        b = (1 / self.train_num) * b_com
        return w, b

    def makePred(self, w, b):
        return w * self.x_train + b

    def loss(self, y_pred):
        square_loss = 0
        for y_true, y_p in zip(self.y_train, y_pred):
            square_loss += math.pow((y_true - y_p), 2)
        return square_loss


# Run the Manual Experiment
print("--- 1. Manual Implementation Results ---")
# Simulating: True slope (marginal propensity to consume) = 0.6, Intercept = 2.0
experiment = SimpleLinear(0.6, 2.0, 200)
w_est, b_est = experiment.compute()
y_pred = experiment.makePred(w_est, b_est)
total_loss = experiment.loss(y_pred)

print(f"True Parameters: w={0.6}, b={2.0}")
print(f"Estimated Parameters: w={w_est:.4f}, b={b_est:.4f}")
print(f"Total Sum of Squared Errors: {total_loss:.4f}")
print(f"Mean Squared Error: {total_loss / 200:.4f}")
print("\n")

# 2. Complete Analysis using Statsmodels (for t-test, F-test, R2)
# The PDF requires analyzing significance, which the manual code doesn't provide.
print("--- 2. Comprehensive Model Inspection (Statsmodels) ---")
X = experiment.x_train
y = experiment.y_train

# Add constant for intercept
X_with_const = sm.add_constant(X)

# Fit OLS model
model = sm.OLS(y, X_with_const)
results = model.fit()

print(results.summary())

# Plotting
plt.figure(figsize=(10, 6))
plt.scatter(X, y, alpha=0.5, label='Actual Data (Synthetic GDP vs Consumption)')
plt.plot(X, y_pred, color='red', label=f'Fitted Line: y = {w_est:.2f}x + {b_est:.2f}')
plt.title('Linear Model Experiment: GDP vs Consumption')
plt.xlabel('GDP (Normalized)')
plt.ylabel('Consumption Level')
plt.legend()
plt.grid(True)
plt.savefig('linear_model_experiment.png')