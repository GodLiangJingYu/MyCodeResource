import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import matplotlib.pyplot as plt

P = np.arange(-1, 1.05, 0.05)
T = np.sin(np.pi * P)

X = torch.from_numpy(P).float().unsqueeze(1)
Y = torch.from_numpy(T).float().unsqueeze(1)


class BPNetwork(nn.Module):
    def __init__(self):
        super(BPNetwork, self).__init__()
        self.hidden = nn.Linear(1, 10)
        self.tanh = nn.Tanh()
        self.output = nn.Linear(10, 1)

    def forward(self, x):
        x = self.hidden(x)
        x = self.tanh(x)
        x = self.output(x)
        return x


net = BPNetwork()

criterion = nn.MSELoss()
optimizer = optim.Adam(net.parameters(), lr=0.01)

num_epochs = 2000

for epoch in range(num_epochs):
    outputs = net(X)
    loss = criterion(outputs, Y)

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

net.eval()
with torch.no_grad():
    predicted = net(X).numpy()

plt.figure(figsize=(10, 6))
plt.plot(P, T, label='Target', color='blue')
plt.plot(P, predicted, label='Predicted', color='red', linestyle='--')
plt.title('BP Neural Network Function Approximation')
plt.xlabel('Input')
plt.ylabel('Output')
plt.legend()
plt.grid(True)
plt.show()