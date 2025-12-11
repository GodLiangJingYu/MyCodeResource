import numpy as np
import matplotlib.pyplot as plt
from PIL import Image
import os
from scipy.ndimage import convolve

def gaussian_filter_kernel(size, sigma):
    """生成高斯滤波核"""
    kernel = np.zeros((size, size), dtype=np.float32)
    center = size // 2
    
    # 计算高斯核
    for i in range(size):
        for j in range(size):
            x, y = i - center, j - center
            kernel[i, j] = np.exp(-(x**2 + y**2) / (2 * sigma**2))
    
    # 归一化
    kernel /= np.sum(kernel)
    return kernel

def laplacian_of_gaussian_kernel(size, sigma):
    """生成高斯拉普拉斯核"""
    kernel = np.zeros((size, size), dtype=np.float32)
    center = size // 2
    
    # 计算高斯拉普拉斯核
    for i in range(size):
        for j in range(size):
            x, y = i - center, j - center
            r_squared = x**2 + y**2
            if 2*np.pi*sigma**6 == 0:
                kernel[i, j] = 0
            else:
                kernel[i, j] = (r_squared - 2*sigma**2) / (2*np.pi*sigma**6) * np.exp(-r_squared / (2*sigma**2))
    
    return kernel

def task1_filtering_edge_detection():
    """任务一：基本处理 - 滤波和边缘检测"""
    print("开始任务一：基本处理 - 滤波和边缘检测")
    
    # 读取lena图像
    lena_gray_path = r"D:\\MyCodeResource\\计算机视觉\\实验\\第一次作业数据集\\lena\\lena512.bmp"
    lena_color_path = r"D:\\MyCodeResource\\计算机视觉\\实验\\第一次作业数据集\\lena\\lena512color.tiff"
    
    if not os.path.exists(lena_gray_path):
        print(f"警告：找不到文件 {lena_gray_path}")
        return
    if not os.path.exists(lena_color_path):
        print(f"警告：找不到文件 {lena_color_path}")
        return
    
    # 使用PIL读取图像
    try:
        lena_gray_pil = Image.open(lena_gray_path)
        lena_gray = np.array(lena_gray_pil)
        
        lena_color_pil = Image.open(lena_color_path)
        lena_color = np.array(lena_color_pil)
        
        print(f"成功读取图像: 灰度图像 {lena_gray.shape}, 彩色图像 {lena_color.shape}")
    except Exception as e:
        print(f"读取图像时出错: {e}")
        return
    
    # 为了快速演示，只使用一个小的子区域
    # 取图像中心的128x128区域
    center_h, center_w = lena_gray.shape[0]//2, lena_gray.shape[1]//2
    half_size = 128
    gray_roi = lena_gray[center_h-half_size:center_h+half_size, center_w-half_size:center_w+half_size]
    color_roi = lena_color[center_h-half_size:center_h+half_size, center_w-half_size:center_w+half_size, :]
    
    print(f"处理ROI: 灰度 {gray_roi.shape}, 彩色 {color_roi.shape}")
    
    # 使用一个参数组合进行演示
    sigma = 1
    size = 5
    print(f"使用参数: 窗口大小={size}, sigma={sigma}")
    
    # 生成滤波核
    gaussian_kernel = gaussian_filter_kernel(size, sigma)
    log_kernel = laplacian_of_gaussian_kernel(size, sigma)
    
    print("应用高斯滤波...")
    filtered_gray = convolve(gray_roi.astype(np.float32), gaussian_kernel, mode='reflect')
    
    print("应用LoG边缘检测...")
    edge_gray = convolve(gray_roi.astype(np.float32), log_kernel, mode='reflect')
    edge_intensity = np.abs(edge_gray)
    
    # 显示结果
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    axes[0].imshow(gray_roi, cmap='gray')
    axes[0].set_title('Original Gray ROI')
    axes[0].axis('off')
    
    axes[1].imshow(filtered_gray, cmap='gray')
    axes[1].set_title(f'Gaussian Filter\nSize:{size}, Sigma:{sigma}')
    axes[1].axis('off')
    
    axes[2].imshow(edge_intensity, cmap='gray')
    axes[2].set_title(f'LoG Edge Intensity\nSize:{size}, Sigma:{sigma}')
    axes[2].axis('off')
    
    plt.tight_layout()
    plt.savefig('任务一结果/task1_gray_results_demo.png', dpi=150, bbox_inches='tight')
    plt.show()
    
    print("处理彩色图像ROI...")
    # 处理彩色图像的每个通道
    filtered_color = np.zeros_like(color_roi, dtype=np.float32)
    
    for c in range(color_roi.shape[2]):
        filtered_color[:, :, c] = convolve(color_roi[:, :, c].astype(np.float32), gaussian_kernel, mode='reflect')
    
    edge_intensity_color = np.zeros_like(color_roi, dtype=np.float32)
    
    for c in range(color_roi.shape[2]):
        edge_channel = convolve(color_roi[:, :, c].astype(np.float32), log_kernel, mode='reflect')
        edge_intensity_color[:, :, c] = np.abs(edge_channel)
    
    edge_intensity_combined = np.sqrt(np.sum(edge_intensity_color**2, axis=2))
    
    # 显示彩色结果
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    axes[0].imshow(color_roi)
    axes[0].set_title('Original Color ROI')
    axes[0].axis('off')
    
    axes[1].imshow(filtered_color.astype(np.uint8))
    axes[1].set_title(f'Color Gaussian Filter\nSize:{size}, Sigma:{sigma}')
    axes[1].axis('off')
    
    axes[2].imshow(edge_intensity_combined, cmap='gray')
    axes[2].set_title(f'Color LoG Edge Intensity\nSize:{size}, Sigma:{sigma}')
    axes[2].axis('off')
    
    plt.tight_layout()
    plt.savefig('任务一结果/task1_color_results_demo.png', dpi=150, bbox_inches='tight')
    plt.show()
    
    print("任务一完成！结果已保存到任务一结果文件夹")

if __name__ == "__main__":
    task1_filtering_edge_detection()