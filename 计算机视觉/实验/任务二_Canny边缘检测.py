import numpy as np
import cv2
import matplotlib.pyplot as plt
from PIL import Image
import os

def task2_canny_edge_detection():
    """任务二：Canny边缘检测"""
    print("开始任务二：Canny边缘检测")
    
    # 读取lena灰度图像
    lena_gray_path = r"D:\MyCodeResource\计算机视觉\实验\第一次作业数据集\lena\lena512.bmp"
    
    if not os.path.exists(lena_gray_path):
        print(f"警告：找不到文件 {lena_gray_path}")
        return
    
    # 使用PIL读取图像
    try:
        lena_gray_pil = Image.open(lena_gray_path)
        lena_gray = np.array(lena_gray_pil)
    except Exception as e:
        print(f"读取图像时出错: {e}")
        return
    
    # 不同参数的Canny边缘检测
    # 参数组合：(低阈值, 高阈值)
    param_combinations = [
        (50, 100),
        (50, 150),
        (50, 200),
        (100, 150),
        (100, 200),
        (100, 250),
        (150, 200),
        (150, 250)
    ]
    
    # 创建结果图 - 需要9个子图 (1个原图 + 8个处理结果)
    fig, axes = plt.subplots(3, 3, figsize=(15, 12))  # 3x3 = 9个子图
    axes = axes.flatten()
    
    # 显示原图
    axes[0].imshow(lena_gray, cmap='gray')
    axes[0].set_title('Original Image')
    axes[0].axis('off')
    
    # 应用不同参数的Canny检测
    for i, (low_thresh, high_thresh) in enumerate(param_combinations):
        canny_edges = cv2.Canny(lena_gray, low_thresh, high_thresh)
        
        axes[i+1].imshow(canny_edges, cmap='gray')
        axes[i+1].set_title(f'Canny (Low:{low_thresh}, High:{high_thresh})')
        axes[i+1].axis('off')
    
    plt.tight_layout()
    plt.savefig('任务二结果/task2_canny_results.png', dpi=150, bbox_inches='tight')
    plt.show()
    
    print("任务二完成！结果已保存到任务二结果文件夹")
    
    # 简单解释Canny边缘检测算法的原理
    print("\nCanny边缘检测算法原理简述：")
    print("1. 高斯滤波：平滑图像，去除噪声")
    print("2. 计算梯度：使用Sobel算子计算像素梯度的幅值和方向")
    print("3. 非极大值抑制：细化边缘，保留局部最大值")
    print("4. 双阈值检测：使用高低阈值连接边缘")
    print("5. 边缘连接：根据阈值连接边缘")

if __name__ == "__main__":
    task2_canny_edge_detection()