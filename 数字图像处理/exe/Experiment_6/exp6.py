# -*- coding: utf-8 -*-
"""
实验六 图像的代数运算
Experiment 6: Image Arithmetic Operations
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage import data
import os

# Set working directory
work_dir = os.path.dirname(os.path.abspath(__file__))

# Configure matplotlib for Chinese characters
plt.rcParams['font.sans-serif'] = ['SimHei']  # Use SimHei for Chinese
plt.rcParams['axes.unicode_minus'] = False    # Fix minus sign

def main():
    print("=" * 50)
    print("实验六：图像的代数运算 (Experiment 6: Image Arithmetic Operations)")
    print("=" * 50)
    
    # 1. Load test images (指导书指定: rice.tif, cameraman.tif)
    print("正在加载测试图像...")
    rice_path = 'media/p024_img01.png'
    cam_path = 'media/p024_img02.png'
    img1 = cv2.imread(rice_path, cv2.IMREAD_GRAYSCALE) if os.path.exists(rice_path) else data.camera()
    img2 = cv2.imread(cam_path, cv2.IMREAD_GRAYSCALE) if os.path.exists(cam_path) else data.moon()
    
    # Ensure they are the same size
    if img1.shape != img2.shape:
        img2 = cv2.resize(img2, (img1.shape[1], img1.shape[0]))
        
    print(f"图像加载成功: img1.shape={img1.shape}, img2.shape={img2.shape}")

    # ---------------------------------------------------------
    # 1. 图像加法运算 (Image Addition)
    # ---------------------------------------------------------
    print("执行图像加法运算...")
    # 1.1 两幅图像相加 (cv2.add equivalent to imadd)
    add_img = cv2.add(img1, img2)
    
    # 1.2 图像加常数 (增加亮度)
    c_val = np.ones_like(img1) * 50
    add_const = cv2.add(img1, c_val)
    
    # Plot Addition Results
    fig_add = plt.figure(figsize=(12, 4))
    plt.suptitle("一、图像加法运算", fontsize=16)
    
    plt.subplot(141), plt.imshow(img1, cmap='gray'), plt.title("原图1 (Camera)"), plt.axis('off')
    plt.subplot(142), plt.imshow(img2, cmap='gray'), plt.title("原图2 (Moon)"), plt.axis('off')
    plt.subplot(143), plt.imshow(add_img, cmap='gray'), plt.title("两图相加"), plt.axis('off')
    plt.subplot(144), plt.imshow(add_const, cmap='gray'), plt.title("图1加常数(提亮)"), plt.axis('off')
    
    plt.savefig(os.path.join(work_dir, "results_add.png"), dpi=150, bbox_inches='tight')
    plt.close(fig_add)
    
    # ---------------------------------------------------------
    # 2. 图像减法运算 (Image Subtraction)
    # ---------------------------------------------------------
    print("执行图像减法运算...")
    # 2.1 两幅图像相减
    sub_img = cv2.subtract(img1, img2)
    
    # 2.2 图像减常数 (降低亮度)
    sub_const = cv2.subtract(img1, c_val)
    
    # 2.3 背景去除 (模拟)
    # Create an artificial non-uniform background (e.g., gradient)
    grad = np.linspace(0, 100, img1.shape[1], dtype=np.uint8)
    bg = np.tile(grad, (img1.shape[0], 1))
    img_with_bg = cv2.add(img1, bg)
    # Background removal: img_with_bg - bg
    bg_removed = cv2.subtract(img_with_bg, bg)
    
    # Plot Subtraction Results
    fig_sub = plt.figure(figsize=(15, 4))
    plt.suptitle("二、图像减法运算", fontsize=16)
    
    plt.subplot(151), plt.imshow(img1, cmap='gray'), plt.title("原图1"), plt.axis('off')
    plt.subplot(152), plt.imshow(sub_img, cmap='gray'), plt.title("两图相减"), plt.axis('off')
    plt.subplot(153), plt.imshow(sub_const, cmap='gray'), plt.title("图1减常数(变暗)"), plt.axis('off')
    plt.subplot(154), plt.imshow(img_with_bg, cmap='gray'), plt.title("加背景图像"), plt.axis('off')
    plt.subplot(155), plt.imshow(bg_removed, cmap='gray'), plt.title("背景去除"), plt.axis('off')
    
    plt.savefig(os.path.join(work_dir, "results_sub.png"), dpi=150, bbox_inches='tight')
    plt.close(fig_sub)
    
    # ---------------------------------------------------------
    # 3. 图像乘法运算 (Image Multiplication)
    # ---------------------------------------------------------
    print("执行图像乘法运算...")
    # 3.1 图像乘常数 (缩放亮度)
    # cv2.multiply to multiply by scalar
    mul_const = cv2.multiply(img1, 1.5)
    
    # 3.2 掩模操作
    # Create a circular mask
    mask = np.zeros_like(img1)
    center = (img1.shape[1] // 2, img1.shape[0] // 2)
    radius = min(center[0], center[1], 150)
    cv2.circle(mask, center, radius, 255, -1)
    
    # Apply mask (multiplication)
    mask_float = (mask / 255.0).astype(np.float32)
    img1_float = img1.astype(np.float32)
    masked_img = cv2.multiply(img1_float, mask_float)
    masked_img = np.clip(masked_img, 0, 255).astype(np.uint8)
    
    # Plot Multiplication Results
    fig_mul = plt.figure(figsize=(12, 4))
    plt.suptitle("三、图像乘法运算", fontsize=16)
    
    plt.subplot(141), plt.imshow(img1, cmap='gray'), plt.title("原图1"), plt.axis('off')
    plt.subplot(142), plt.imshow(mul_const, cmap='gray'), plt.title("乘常数(亮度拉伸)"), plt.axis('off')
    plt.subplot(143), plt.imshow(mask, cmap='gray'), plt.title("二值掩模"), plt.axis('off')
    plt.subplot(144), plt.imshow(masked_img, cmap='gray'), plt.title("掩模操作(相乘)"), plt.axis('off')
    
    plt.savefig(os.path.join(work_dir, "results_mul.png"), dpi=150, bbox_inches='tight')
    plt.close(fig_mul)
    
    # ---------------------------------------------------------
    # 4. 图像除法运算 (Image Division)
    # ---------------------------------------------------------
    print("执行图像除法运算...")
    # 4.1 比率变换 (Ratio Transform)
    # Add 1 to avoid division by zero
    div_img = cv2.divide(img1.astype(np.float32), img2.astype(np.float32) + 1.0)
    # Normalize for display
    div_img_norm = cv2.normalize(div_img, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_8U)
    
    fig_div = plt.figure(figsize=(12, 4))
    plt.suptitle("四、图像除法运算", fontsize=16)
    
    plt.subplot(131), plt.imshow(img1, cmap='gray'), plt.title("原图1"), plt.axis('off')
    plt.subplot(132), plt.imshow(img2, cmap='gray'), plt.title("原图2"), plt.axis('off')
    plt.subplot(133), plt.imshow(div_img_norm, cmap='gray'), plt.title("两图相除(比率变换)"), plt.axis('off')
    
    plt.savefig(os.path.join(work_dir, "results_div.png"), dpi=150, bbox_inches='tight')
    plt.close(fig_div)
    
    # ---------------------------------------------------------
    # 5. 图像四则运算 - 线性组合 (Linear Combination)
    # ---------------------------------------------------------
    print("执行图像线性组合...")
    # imlincomb equivalent
    # alpha * img1 + beta * img2 + gamma
    linear_comb = cv2.addWeighted(img1, 0.6, img2, 0.4, 0)
    
    # ---------------------------------------------------------
    # 6. Comprehensive Results
    # ---------------------------------------------------------
    print("生成综合实验结果图...")
    fig_all = plt.figure(figsize=(15, 10))
    plt.suptitle("实验六综合结果：图像的代数运算", fontsize=18)
    
    plt.subplot(241), plt.imshow(img1, cmap='gray'), plt.title("原图1"), plt.axis('off')
    plt.subplot(242), plt.imshow(img2, cmap='gray'), plt.title("原图2"), plt.axis('off')
    plt.subplot(243), plt.imshow(add_img, cmap='gray'), plt.title("加法(融合)"), plt.axis('off')
    plt.subplot(244), plt.imshow(sub_img, cmap='gray'), plt.title("减法(差异)"), plt.axis('off')
    
    plt.subplot(245), plt.imshow(mul_const, cmap='gray'), plt.title("乘法(亮度缩放)"), plt.axis('off')
    plt.subplot(246), plt.imshow(masked_img, cmap='gray'), plt.title("乘法(掩模)"), plt.axis('off')
    plt.subplot(247), plt.imshow(div_img_norm, cmap='gray'), plt.title("除法(比率)"), plt.axis('off')
    plt.subplot(248), plt.imshow(linear_comb, cmap='gray'), plt.title("线性组合(加权融合)"), plt.axis('off')
    
    plt.savefig(os.path.join(work_dir, "results.png"), dpi=150, bbox_inches='tight')
    plt.close(fig_all)
    
    print("=" * 50)
    print("实验六处理完成，所有结果已保存到 Experiment_6 目录下。")
    print("=" * 50)

if __name__ == "__main__":
    main()
