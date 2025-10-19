import os
import requests
import gzip
import numpy as np


def download_mnist():
    url_base = 'http://yann.lecun.com/exdb/mnist/'
    file_names = ['train-images-idx3-ubyte.gz', 'train-labels-idx1-ubyte.gz',
                  't10k-images-idx3-ubyte.gz', 't10k-labels-idx1-ubyte.gz']

    if not os.path.exists('./MNIST_Data'):
        os.mkdir('./MNIST_Data')
    if not os.path.exists('./MNIST_Data/train'):
        os.mkdir('./MNIST_Data/train')
    if not os.path.exists('./MNIST_Data/test'):
        os.mkdir('./MNIST_Data/test')

    for file_name in file_names:
        print(f'Downloading {file_name}...')
        response = requests.get(url_base + file_name)
        with open(f'./MNIST_Data/{file_name}', 'wb') as f:
            f.write(response.content)

    # 解压并格式化数据
    print('Processing data...')
    # 训练图像
    with gzip.open('./MNIST_Data/train-images-idx3-ubyte.gz', 'rb') as f:
        train_images = np.frombuffer(f.read(), np.uint8, offset=16).reshape(-1, 28, 28)
    # 训练标签
    with gzip.open('./MNIST_Data/train-labels-idx1-ubyte.gz', 'rb') as f:
        train_labels = np.frombuffer(f.read(), np.uint8, offset=8)
    # 测试图像
    with gzip.open('./MNIST_Data/t10k-images-idx3-ubyte.gz', 'rb') as f:
        test_images = np.frombuffer(f.read(), np.uint8, offset=16).reshape(-1, 28, 28)
    # 测试标签
    with gzip.open('./MNIST_Data/t10k-labels-idx1-ubyte.gz', 'rb') as f:
        test_labels = np.frombuffer(f.read(), np.uint8, offset=8)

    # 保存为MindSpore可用格式
    np.save('./MNIST_Data/train/train_images.npy', train_images)
    np.save('./MNIST_Data/train/train_labels.npy', train_labels)
    np.save('./MNIST_Data/test/test_images.npy', test_images)
    np.save('./MNIST_Data/test/test_labels.npy', test_labels)

    print('Download and processing complete!')


if __name__ == "__main__":
    download_mnist()