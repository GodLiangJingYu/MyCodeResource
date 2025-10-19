import os
import urllib.request
import gzip
import numpy as np
import struct


def download_mnist_numpy():
    """下载MNIST数据集并保存为.npy格式"""
    print("开始下载MNIST数据集...")

    # 创建目录
    base_dir = './MNIST_Data'
    os.makedirs(base_dir, exist_ok=True)

    # 下载文件
    urls = {
        'train_images': 'http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz',
        'train_labels': 'http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz',
        'test_images': 'http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz',
        'test_labels': 'http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz'
    }

    for name, url in urls.items():
        print(f"下载 {name}...")
        save_path = os.path.join(base_dir, f"{name}.gz")
        try:
            urllib.request.urlretrieve(url, save_path)
        except Exception as e:
            print(f"下载 {name} 时出错: {e}")
            return False

    # 解压并保存为.npy格式
    print("处理训练图像...")
    with gzip.open(os.path.join(base_dir, 'train_images.gz'), 'rb') as f:
        magic, num, rows, cols = struct.unpack('>IIII', f.read(16))
        train_images = np.frombuffer(f.read(), dtype=np.uint8).reshape(num, rows, cols)

    print("处理训练标签...")
    with gzip.open(os.path.join(base_dir, 'train_labels.gz'), 'rb') as f:
        magic, num = struct.unpack('>II', f.read(8))
        train_labels = np.frombuffer(f.read(), dtype=np.uint8)

    print("处理测试图像...")
    with gzip.open(os.path.join(base_dir, 'test_images.gz'), 'rb') as f:
        magic, num, rows, cols = struct.unpack('>IIII', f.read(16))
        test_images = np.frombuffer(f.read(), dtype=np.uint8).reshape(num, rows, cols)

    print("处理测试标签...")
    with gzip.open(os.path.join(base_dir, 'test_labels.gz'), 'rb') as f:
        magic, num = struct.unpack('>II', f.read(8))
        test_labels = np.frombuffer(f.read(), dtype=np.uint8)

    # 保存为.npy文件
    np.save(os.path.join(base_dir, 'train_images.npy'), train_images)
    np.save(os.path.join(base_dir, 'train_labels.npy'), train_labels)
    np.save(os.path.join(base_dir, 'test_images.npy'), test_images)
    np.save(os.path.join(base_dir, 'test_labels.npy'), test_labels)

    print("MNIST数据集下载和处理完成！")
    print(f"文件保存在 {os.path.abspath(base_dir)} 目录中")
    return True


if __name__ == "__main__":
    download_mnist_numpy()