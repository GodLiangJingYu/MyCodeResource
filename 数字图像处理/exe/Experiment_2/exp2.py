import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage.util import random_noise

def main():
    # 指导书指定: electric.tif
    electric_path = 'media/p011_img01.png'
    img = cv2.imread(electric_path, cv2.IMREAD_GRAYSCALE)
    if img is None:
        img = cv2.imread('Experiment_2/lenna.png', cv2.IMREAD_GRAYSCALE)

    print('='*60)
    print('  实验二：图像增强—空域滤波 (Python实现)')
    print('='*60)

    # ===== Step 1: 添加不同强度的噪声 =====
    print('\n--- Step 1: 添加不同强度的噪声 ---')

    # 高斯噪声 - 低强度
    noise_gauss_low = random_noise(img, mode='gaussian', var=0.005)
    noise_gauss_low = np.clip(255 * noise_gauss_low, 0, 255).astype('uint8')

    # 高斯噪声 - 中强度
    noise_gauss_mid = random_noise(img, mode='gaussian', var=0.01)
    noise_gauss_mid = np.clip(255 * noise_gauss_mid, 0, 255).astype('uint8')

    # 高斯噪声 - 高强度
    noise_gauss_high = random_noise(img, mode='gaussian', var=0.05)
    noise_gauss_high = np.clip(255 * noise_gauss_high, 0, 255).astype('uint8')

    # 椒盐噪声 - 低强度
    noise_sp_low = random_noise(img, mode='s&p', amount=0.02)
    noise_sp_low = np.clip(255 * noise_sp_low, 0, 255).astype('uint8')

    # 椒盐噪声 - 中强度
    noise_sp_mid = random_noise(img, mode='s&p', amount=0.05)
    noise_sp_mid = np.clip(255 * noise_sp_mid, 0, 255).astype('uint8')

    # 椒盐噪声 - 高强度
    noise_sp_high = random_noise(img, mode='s&p', amount=0.10)
    noise_sp_high = np.clip(255 * noise_sp_high, 0, 255).astype('uint8')

    print("  高斯噪声: var=0.005, 0.01, 0.05")
    print("  椒盐噪声: amount=0.02, 0.05, 0.10")

    # ===== Step 2: 不同模板大小的均值滤波 =====
    print('\n--- Step 2: 均值滤波 (不同模板大小) ---')

    # 对中等强度高斯噪声图像进行不同大小的均值滤波
    gauss_mean_3x3 = cv2.blur(noise_gauss_mid, (3, 3))
    gauss_mean_5x5 = cv2.blur(noise_gauss_mid, (5, 5))

    # 对中等强度椒盐噪声图像进行不同大小的均值滤波
    sp_mean_3x3 = cv2.blur(noise_sp_mid, (3, 3))
    sp_mean_5x5 = cv2.blur(noise_sp_mid, (5, 5))

    print("  模板大小: 3×3, 5×5")

    # ===== Step 3: 不同模板大小的中值滤波 =====
    print('\n--- Step 3: 中值滤波 (不同模板大小) ---')

    # 对中等强度高斯噪声图像进行不同大小的中值滤波
    gauss_median_3x3 = cv2.medianBlur(noise_gauss_mid, 3)
    gauss_median_4x4 = cv2.medianBlur(noise_gauss_mid, 5)  # medfilt2不支持偶数，取最近奇数5

    # 对中等强度椒盐噪声图像进行不同大小的中值滤波
    sp_median_3x3 = cv2.medianBlur(noise_sp_mid, 3)
    sp_median_4x4 = cv2.medianBlur(noise_sp_mid, 5)

    # 指导书要求[4 4]模板，Python的medianBlur只接受奇数内核，用5近似
    print("  模板大小: 3×3, 4×4(近似为5×5)")

    # ===== Step 4: 绘制结果图 - 噪声对比 =====
    print('\n--- Step 4: 绘制结果图 ---')

    # 图1: 不同噪声类型和强度
    fig1, axs1 = plt.subplots(2, 4, figsize=(20, 10))
    fig1.suptitle('不同噪声类型与强度对比', fontsize=16)

    axs1[0, 0].imshow(img, cmap='gray')
    axs1[0, 0].set_title('Original')
    axs1[0, 0].axis('off')

    axs1[0, 1].imshow(noise_gauss_low, cmap='gray')
    axs1[0, 1].set_title('Gaussian (var=0.005)')
    axs1[0, 1].axis('off')

    axs1[0, 2].imshow(noise_gauss_mid, cmap='gray')
    axs1[0, 2].set_title('Gaussian (var=0.01)')
    axs1[0, 2].axis('off')

    axs1[0, 3].imshow(noise_gauss_high, cmap='gray')
    axs1[0, 3].set_title('Gaussian (var=0.05)')
    axs1[0, 3].axis('off')

    axs1[1, 0].imshow(img, cmap='gray')
    axs1[1, 0].set_title('Original')
    axs1[1, 0].axis('off')

    axs1[1, 1].imshow(noise_sp_low, cmap='gray')
    axs1[1, 1].set_title('Salt&Pepper (2%)')
    axs1[1, 1].axis('off')

    axs1[1, 2].imshow(noise_sp_mid, cmap='gray')
    axs1[1, 2].set_title('Salt&Pepper (5%)')
    axs1[1, 2].axis('off')

    axs1[1, 3].imshow(noise_sp_high, cmap='gray')
    axs1[1, 3].set_title('Salt&Pepper (10%)')
    axs1[1, 3].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_2/results_noise.png', dpi=150)
    print("  噪声对比图已保存: results_noise.png")

    # 图2: 高斯噪声滤波对比
    fig2, axs2 = plt.subplots(2, 3, figsize=(15, 10))
    fig2.suptitle('高斯噪声 (var=0.01) 滤波效果对比', fontsize=16)

    axs2[0, 0].imshow(img, cmap='gray')
    axs2[0, 0].set_title('Original')
    axs2[0, 0].axis('off')

    axs2[0, 1].imshow(noise_gauss_mid, cmap='gray')
    axs2[0, 1].set_title('Gaussian Noise')
    axs2[0, 1].axis('off')

    axs2[0, 2].axis('off')

    axs2[1, 0].imshow(gauss_mean_3x3, cmap='gray')
    axs2[1, 0].set_title('Mean Filter 3×3')
    axs2[1, 0].axis('off')

    axs2[1, 1].imshow(gauss_mean_5x5, cmap='gray')
    axs2[1, 1].set_title('Mean Filter 5×5')
    axs2[1, 1].axis('off')

    axs2[1, 2].imshow(gauss_median_3x3, cmap='gray')
    axs2[1, 2].set_title('Median Filter 3×3')
    axs2[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_2/results_gaussian.png', dpi=150)
    print("  高斯噪声滤波对比图已保存: results_gaussian.png")

    # 图3: 椒盐噪声滤波对比
    fig3, axs3 = plt.subplots(2, 3, figsize=(15, 10))
    fig3.suptitle('椒盐噪声 (5%) 滤波效果对比', fontsize=16)

    axs3[0, 0].imshow(img, cmap='gray')
    axs3[0, 0].set_title('Original')
    axs3[0, 0].axis('off')

    axs3[0, 1].imshow(noise_sp_mid, cmap='gray')
    axs3[0, 1].set_title('Salt&Pepper Noise')
    axs3[0, 1].axis('off')

    axs3[0, 2].axis('off')

    axs3[1, 0].imshow(sp_mean_3x3, cmap='gray')
    axs3[1, 0].set_title('Mean Filter 3×3')
    axs3[1, 0].axis('off')

    axs3[1, 1].imshow(sp_mean_5x5, cmap='gray')
    axs3[1, 1].set_title('Mean Filter 5×5')
    axs3[1, 1].axis('off')

    axs3[1, 2].imshow(sp_median_3x3, cmap='gray')
    axs3[1, 2].set_title('Median Filter 3×3')
    axs3[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_2/results_saltpepper.png', dpi=150)
    print("  椒盐噪声滤波对比图已保存: results_saltpepper.png")

    # 图4: 综合对比 (合并为一张大图)
    fig4, axs4 = plt.subplots(3, 5, figsize=(20, 12))
    fig4.suptitle('空域滤波综合对比', fontsize=16)

    # 第一行: 原图 + 噪声图
    axs4[0, 0].imshow(img, cmap='gray')
    axs4[0, 0].set_title('Original')
    axs4[0, 0].axis('off')

    axs4[0, 1].imshow(noise_gauss_mid, cmap='gray')
    axs4[0, 1].set_title('Gaussian\n(var=0.01)')
    axs4[0, 1].axis('off')

    axs4[0, 2].imshow(noise_sp_mid, cmap='gray')
    axs4[0, 2].set_title('Salt&Pepper\n(5%)')
    axs4[0, 2].axis('off')

    axs4[0, 3].axis('off')
    axs4[0, 4].axis('off')

    # 第二行: 高斯噪声滤波结果
    axs4[1, 0].imshow(gauss_mean_3x3, cmap='gray')
    axs4[1, 0].set_title('Gauss+Mean\n3×3')
    axs4[1, 0].axis('off')

    axs4[1, 1].imshow(gauss_mean_5x5, cmap='gray')
    axs4[1, 1].set_title('Gauss+Mean\n5×5')
    axs4[1, 1].axis('off')

    axs4[1, 2].imshow(gauss_median_3x3, cmap='gray')
    axs4[1, 2].set_title('Gauss+Median\n3×3')
    axs4[1, 2].axis('off')

    axs4[1, 3].imshow(gauss_median_4x4, cmap='gray')
    axs4[1, 3].set_title('Gauss+Median\n4×4(≈5×5)')
    axs4[1, 3].axis('off')

    axs4[1, 4].axis('off')

    # 第三行: 椒盐噪声滤波结果
    axs4[2, 0].imshow(sp_mean_3x3, cmap='gray')
    axs4[2, 0].set_title('S&P+Mean\n3×3')
    axs4[2, 0].axis('off')

    axs4[2, 1].imshow(sp_mean_5x5, cmap='gray')
    axs4[2, 1].set_title('S&P+Mean\n5×5')
    axs4[2, 1].axis('off')

    axs4[2, 2].imshow(sp_median_3x3, cmap='gray')
    axs4[2, 2].set_title('S&P+Median\n3×3')
    axs4[2, 2].axis('off')

    axs4[2, 3].imshow(sp_median_4x4, cmap='gray')
    axs4[2, 3].set_title('S&P+Median\n4×4(≈5×5)')
    axs4[2, 3].axis('off')

    axs4[2, 4].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_2/results.png', dpi=150)
    print("  综合对比图已保存: results.png")

    print("\n实验二完成。")

if __name__ == '__main__':
    main()
