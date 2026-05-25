import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage import data
import os

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# 确保输出目录存在
output_dir = os.path.dirname(os.path.abspath(__file__))

def plot_image_and_hist(image, title, save_path=None):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
    
    # 显示图像
    ax1.imshow(image, cmap='gray')
    ax1.set_title(title)
    ax1.axis('off')
    
    # 计算直方图
    hist, bins = np.histogram(image.flatten(), 256, [0, 256])
    # 计算累积分布函数(CDF)
    cdf = hist.cumsum()
    cdf_normalized = cdf * float(hist.max()) / cdf.max()
    
    # 绘制直方图和CDF
    ax2.plot(cdf_normalized, color='b', label='CDF')
    ax2.hist(image.flatten(), 256, range=[0, 256], color='r', alpha=0.7, label='直方图')
    ax2.set_xlim([0, 256])
    ax2.legend(loc='upper left')
    ax2.set_title(f'{title} - 直方图与CDF')
    ax2.set_xlabel('灰度值')
    ax2.set_ylabel('像素数量')
    
    plt.tight_layout()
    if save_path:
        plt.savefig(save_path, dpi=150)
    plt.close()

def main():
    # 1. 读入原图像 (指导书指定: pollen.jpg)
    pollen_path = 'media/p034_img01.png'
    if os.path.exists(pollen_path):
        img = cv2.imread(pollen_path, cv2.IMREAD_GRAYSCALE)
    else:
        try:
            img = data.pollen()
        except AttributeError:
            img = data.moon()

    if img is None:
        try:
            img = data.pollen()
        except AttributeError:
            img = data.moon()
        
    if len(img.shape) == 3:
        img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
        
    # 保存原始图像直方图
    plot_image_and_hist(img, '原图像', os.path.join(output_dir, 'results_histogram.png'))
    
    # 2. 直方图均衡化处理
    img_eq = cv2.equalizeHist(img)
    plot_image_and_hist(img_eq, '全局直方图均衡化', os.path.join(output_dir, 'results_equalized.png'))
    
    # 3. 限制对比度自适应直方图均衡化 (CLAHE)
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    img_clahe = clahe.apply(img)
    plot_image_and_hist(img_clahe, 'CLAHE自适应直方图均衡化', os.path.join(output_dir, 'results_clahe.png'))
    
    # 4. 综合对比图 (results.png)
    fig, axes = plt.subplots(3, 2, figsize=(12, 12))
    images = [(img, '原图像'), (img_eq, '全局直方图均衡化'), (img_clahe, 'CLAHE自适应直方图均衡化')]
    
    for i, (image, title) in enumerate(images):
        # 图像
        axes[i, 0].imshow(image, cmap='gray')
        axes[i, 0].set_title(title)
        axes[i, 0].axis('off')
        
        # 直方图与CDF
        hist, bins = np.histogram(image.flatten(), 256, [0, 256])
        cdf = hist.cumsum()
        cdf_normalized = cdf * float(hist.max()) / cdf.max()
        
        axes[i, 1].plot(cdf_normalized, color='b', label='CDF')
        axes[i, 1].hist(image.flatten(), 256, range=[0, 256], color='r', alpha=0.7, label='直方图')
        axes[i, 1].set_xlim([0, 256])
        axes[i, 1].legend(loc='upper left')
        axes[i, 1].set_title(f'{title} - 直方图与CDF')
    
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, 'results.png'), dpi=150)
    plt.close()

if __name__ == "__main__":
    main()
