import cv2
import numpy as np
import matplotlib.pyplot as plt

def create_ideal_lowpass(shape, d0):
    """理想低通滤波器"""
    rows, cols = shape
    crow, ccol = rows // 2, cols // 2
    mask = np.zeros((rows, cols), np.float64)
    y, x = np.ogrid[-crow:rows-crow, -ccol:cols-ccol]
    mask[x**2 + y**2 <= d0**2] = 1
    return mask

def create_butterworth_lowpass(shape, d0, n=2):
    """巴特沃斯低通滤波器"""
    rows, cols = shape
    crow, ccol = rows // 2, cols // 2
    y, x = np.ogrid[-crow:rows-crow, -ccol:cols-ccol]
    d = np.sqrt(x**2 + y**2)
    mask = 1 / (1 + (d / d0)**(2 * n))
    return mask

def create_gaussian_lowpass(shape, d0):
    """高斯低通滤波器"""
    rows, cols = shape
    crow, ccol = rows // 2, cols // 2
    y, x = np.ogrid[-crow:rows-crow, -ccol:cols-ccol]
    d = np.sqrt(x**2 + y**2)
    mask = np.exp(-(d**2) / (2 * (d0**2)))
    return mask

def create_highpass_from_lowpass(lowpass_mask):
    """由低通滤波器生成对应的高通滤波器"""
    return 1 - lowpass_mask

def main():
    # 指导书指定: room.tif
    room_path = 'media/p014_img01.png'
    img = cv2.imread(room_path, cv2.IMREAD_GRAYSCALE)
    if img is None:
        img = cv2.imread('Experiment_3/lenna.png', cv2.IMREAD_GRAYSCALE)

    print('='*60)
    print('  实验三：图像增强—频域滤波 (Python实现)')
    print('='*60)

    # ===== Step 1: 傅立叶变换与频谱显示 =====
    print('\n--- Step 1: 傅立叶变换与频谱显示 ---')
    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)
    magnitude_spectrum = 20 * np.log(np.abs(fshift) + 1)
    print("  2D FFT完成，频谱已中心化")

    # ===== Step 2: 设计低通滤波器 =====
    print('\n--- Step 2: 设计低通滤波器 ---')
    d0 = 50  # 截止频率

    ilpf_mask = create_ideal_lowpass(img.shape, d0)
    blpf_mask = create_butterworth_lowpass(img.shape, d0, n=2)
    glpf_mask = create_gaussian_lowpass(img.shape, d0)

    print(f"  截止频率 D0 = {d0}")
    print("  已创建: 理想低通(ILPF)、巴特沃斯低通(BLPF)、高斯低通(GLPF)")

    # ===== Step 3: 设计高通滤波器 =====
    print('\n--- Step 3: 设计高通滤波器 ---')
    ihpf_mask = create_highpass_from_lowpass(ilpf_mask)
    bhpf_mask = create_highpass_from_lowpass(blpf_mask)
    ghpf_mask = create_highpass_from_lowpass(glpf_mask)
    print("  已创建: 理想高通(IHPF)、巴特沃斯高通(BHPF)、高斯高通(GHPF)")

    # ===== Step 4: 应用低通滤波器 =====
    print('\n--- Step 4: 应用低通滤波器 ---')
    # ILPF (低通滤波后无负值，直接取实部截断)
    fshift_ilpf = fshift * ilpf_mask
    f_ishift_ilpf = np.fft.ifftshift(fshift_ilpf)
    img_back_ilpf = np.clip(np.real(np.fft.ifft2(f_ishift_ilpf)), 0, 255).astype(np.uint8)

    # BLPF
    fshift_blpf = fshift * blpf_mask
    f_ishift_blpf = np.fft.ifftshift(fshift_blpf)
    img_back_blpf = np.clip(np.real(np.fft.ifft2(f_ishift_blpf)), 0, 255).astype(np.uint8)

    # GLPF
    fshift_glpf = fshift * glpf_mask
    f_ishift_glpf = np.fft.ifftshift(fshift_glpf)
    img_back_glpf = np.clip(np.real(np.fft.ifft2(f_ishift_glpf)), 0, 255).astype(np.uint8)

    print("  低通滤波完成: ILPF, BLPF, GLPF")

    # ===== Step 5: 应用高通滤波器 =====
    print('\n--- Step 5: 应用高通滤波器 ---')
    # IHPF (高通滤波后保留边缘极性：取实部后截断到[0,255])
    fshift_ihpf = fshift * ihpf_mask
    f_ishift_ihpf = np.fft.ifftshift(fshift_ihpf)
    img_back_ihpf = np.clip(np.real(np.fft.ifft2(f_ishift_ihpf)), 0, 255).astype(np.uint8)

    # BHPF
    fshift_bhpf = fshift * bhpf_mask
    f_ishift_bhpf = np.fft.ifftshift(fshift_bhpf)
    img_back_bhpf = np.clip(np.real(np.fft.ifft2(f_ishift_bhpf)), 0, 255).astype(np.uint8)

    # GHPF
    fshift_ghpf = fshift * ghpf_mask
    f_ishift_ghpf = np.fft.ifftshift(fshift_ghpf)
    img_back_ghpf = np.clip(np.real(np.fft.ifft2(f_ishift_ghpf)), 0, 255).astype(np.uint8)

    print("  高通滤波完成: IHPF, BHPF, GHPF")

    # ===== Step 6: 空域滤波对比 =====
    print('\n--- Step 6: 空域滤波对比 ---')
    # 使用高斯滤波器进行空域滤波
    img_gauss_spatial = cv2.GaussianBlur(img, (15, 15), 0)
    # 使用Sobel算子进行空域高通滤波
    sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=3)
    sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=3)
    img_sobel = np.sqrt(sobelx**2 + sobely**2)
    img_sobel = np.uint8(np.clip(img_sobel, 0, 255))
    print("  空域高斯滤波和Sobel边缘检测完成")

    # ===== Step 7: 绘制结果图 =====
    print('\n--- Step 7: 绘制结果图 ---')

    # 图1: 低通滤波结果
    fig1, axs1 = plt.subplots(2, 3, figsize=(18, 12))
    fig1.suptitle('频域低通滤波结果', fontsize=16)

    axs1[0, 0].imshow(img, cmap='gray')
    axs1[0, 0].set_title('Original Image')
    axs1[0, 0].axis('off')

    axs1[0, 1].imshow(magnitude_spectrum, cmap='gray')
    axs1[0, 1].set_title('Magnitude Spectrum')
    axs1[0, 1].axis('off')

    axs1[0, 2].imshow(ilpf_mask, cmap='gray')
    axs1[0, 2].set_title(f'ILPF Mask (D0={d0})')
    axs1[0, 2].axis('off')

    axs1[1, 0].imshow(img_back_ilpf, cmap='gray')
    axs1[1, 0].set_title('ILPF Result')
    axs1[1, 0].axis('off')

    axs1[1, 1].imshow(img_back_blpf, cmap='gray')
    axs1[1, 1].set_title(f'BLPF Result (n=2)')
    axs1[1, 1].axis('off')

    axs1[1, 2].imshow(img_back_glpf, cmap='gray')
    axs1[1, 2].set_title('GLPF Result')
    axs1[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_3/results_lowpass.png', dpi=150)
    print("  低通滤波结果图已保存: results_lowpass.png")

    # 图2: 高通滤波结果
    fig2, axs2 = plt.subplots(2, 3, figsize=(18, 12))
    fig2.suptitle('频域高通滤波结果', fontsize=16)

    axs2[0, 0].imshow(img, cmap='gray')
    axs2[0, 0].set_title('Original Image')
    axs2[0, 0].axis('off')

    axs2[0, 1].imshow(ihpf_mask, cmap='gray')
    axs2[0, 1].set_title(f'IHPF Mask (D0={d0})')
    axs2[0, 1].axis('off')

    axs2[0, 2].imshow(bhpf_mask, cmap='gray')
    axs2[0, 2].set_title(f'BHPF Mask (n=2)')
    axs2[0, 2].axis('off')

    axs2[1, 0].imshow(img_back_ihpf, cmap='gray')
    axs2[1, 0].set_title('IHPF Result')
    axs2[1, 0].axis('off')

    axs2[1, 1].imshow(img_back_bhpf, cmap='gray')
    axs2[1, 1].set_title('BHPF Result')
    axs2[1, 1].axis('off')

    axs2[1, 2].imshow(img_back_ghpf, cmap='gray')
    axs2[1, 2].set_title('GHPF Result')
    axs2[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_3/results_highpass.png', dpi=150)
    print("  高通滤波结果图已保存: results_highpass.png")

    # 图3: 频域与空域对比
    fig3, axs3 = plt.subplots(2, 3, figsize=(18, 12))
    fig3.suptitle('频域滤波与空域滤波对比', fontsize=16)

    axs3[0, 0].imshow(img, cmap='gray')
    axs3[0, 0].set_title('Original')
    axs3[0, 0].axis('off')

    axs3[0, 1].imshow(img_back_glpf, cmap='gray')
    axs3[0, 1].set_title('GLPF (Frequency Domain)')
    axs3[0, 1].axis('off')

    axs3[0, 2].imshow(img_gauss_spatial, cmap='gray')
    axs3[0, 2].set_title('Gaussian Blur (Spatial Domain)')
    axs3[0, 2].axis('off')

    axs3[1, 0].imshow(img, cmap='gray')
    axs3[1, 0].set_title('Original')
    axs3[1, 0].axis('off')

    axs3[1, 1].imshow(img_back_ghpf, cmap='gray')
    axs3[1, 1].set_title('GHPF (Frequency Domain)')
    axs3[1, 1].axis('off')

    axs3[1, 2].imshow(img_sobel, cmap='gray')
    axs3[1, 2].set_title('Sobel Edge (Spatial Domain)')
    axs3[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_3/results_comparison.png', dpi=150)
    print("  频域/空域对比图已保存: results_comparison.png")

    # 图4: 综合结果 (合并为一张大图)
    fig4, axs4 = plt.subplots(3, 4, figsize=(20, 15))
    fig4.suptitle('频域滤波综合结果', fontsize=16)

    # 第一行: 原图、频谱、滤波器
    axs4[0, 0].imshow(img, cmap='gray')
    axs4[0, 0].set_title('Original')
    axs4[0, 0].axis('off')

    axs4[0, 1].imshow(magnitude_spectrum, cmap='gray')
    axs4[0, 1].set_title('Magnitude Spectrum')
    axs4[0, 1].axis('off')

    axs4[0, 2].imshow(ilpf_mask, cmap='gray')
    axs4[0, 2].set_title('ILPF Mask')
    axs4[0, 2].axis('off')

    axs4[0, 3].imshow(ihpf_mask, cmap='gray')
    axs4[0, 3].set_title('IHPF Mask')
    axs4[0, 3].axis('off')

    # 第二行: 低通结果
    axs4[1, 0].imshow(img_back_ilpf, cmap='gray')
    axs4[1, 0].set_title('ILPF')
    axs4[1, 0].axis('off')

    axs4[1, 1].imshow(img_back_blpf, cmap='gray')
    axs4[1, 1].set_title('BLPF (n=2)')
    axs4[1, 1].axis('off')

    axs4[1, 2].imshow(img_back_glpf, cmap='gray')
    axs4[1, 2].set_title('GLPF')
    axs4[1, 2].axis('off')

    axs4[1, 3].imshow(img_gauss_spatial, cmap='gray')
    axs4[1, 3].set_title('Spatial Gaussian')
    axs4[1, 3].axis('off')

    # 第三行: 高通结果
    axs4[2, 0].imshow(img_back_ihpf, cmap='gray')
    axs4[2, 0].set_title('IHPF')
    axs4[2, 0].axis('off')

    axs4[2, 1].imshow(img_back_bhpf, cmap='gray')
    axs4[2, 1].set_title('BHPF')
    axs4[2, 1].axis('off')

    axs4[2, 2].imshow(img_back_ghpf, cmap='gray')
    axs4[2, 2].set_title('GHPF')
    axs4[2, 2].axis('off')

    axs4[2, 3].imshow(img_sobel, cmap='gray')
    axs4[2, 3].set_title('Sobel Edge')
    axs4[2, 3].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_3/results.png', dpi=150)
    print("  综合结果图已保存: results.png")

    print("\n实验三完成。")

if __name__ == '__main__':
    main()
