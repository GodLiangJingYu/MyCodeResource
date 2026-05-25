import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage import data
import os

# Set Chinese font
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

def show_img_hist(img, title, subplot_idx, fig):
    plt.subplot(*subplot_idx)
    if len(img.shape) == 2:
        plt.imshow(img, cmap='gray', vmin=0, vmax=255)
    else:
        plt.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    plt.title(title)
    plt.axis('off')

    plt.subplot(subplot_idx[0], subplot_idx[1], subplot_idx[2] + 1)
    plt.hist(img.ravel(), bins=256, range=[0, 256])
    plt.title(f"{title} - 直方图")
    plt.xlim([0, 256])

def main():
    save_dir = 'Experiment_7'
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)
        
    # 1. Image data read (指导书指定: medicine_pic.jpg, point.jpg)
    medicine_path = 'media/p031_img02.png'
    point_path = 'media/p031_img01.png'
    img_camera = cv2.imread(medicine_path, cv2.IMREAD_GRAYSCALE) if os.path.exists(medicine_path) else data.camera()
    img_moon = cv2.imread(point_path, cv2.IMREAD_GRAYSCALE) if os.path.exists(point_path) else data.moon()

    # 3. Gray-level reversal (Negative image)
    img_negative = 255 - img_camera
    
    plt.figure(figsize=(12, 8))
    show_img_hist(img_camera, '原始图像', (2, 2, 1), plt.gcf())
    show_img_hist(img_negative, '灰度反转(负片)', (2, 2, 3), plt.gcf())
    plt.tight_layout()
    plt.savefig(os.path.join(save_dir, 'results_negative.png'), dpi=150, bbox_inches='tight')
    plt.close()

    # 4. Gray-level stretching
    min_in = 0.3 * 255
    max_in = 0.6 * 255
    img_stretch = np.copy(img_camera).astype(float)
    img_stretch[img_stretch < min_in] = min_in
    img_stretch[img_stretch > max_in] = max_in
    img_stretch = (img_stretch - min_in) / (max_in - min_in) * 255
    img_stretch = img_stretch.astype(np.uint8)

    plt.figure(figsize=(12, 8))
    show_img_hist(img_camera, '原始图像', (2, 2, 1), plt.gcf())
    show_img_hist(img_stretch, '灰度拉伸', (2, 2, 3), plt.gcf())
    plt.tight_layout()
    plt.savefig(os.path.join(save_dir, 'results_stretch.png'), dpi=150, bbox_inches='tight')
    plt.close()

    # 5. Logarithmic transformation
    c = 255.0 / np.log(1 + 255)
    img_log = c * np.log(1 + img_moon.astype(float))
    img_log = np.uint8(img_log)

    plt.figure(figsize=(12, 8))
    show_img_hist(img_moon, '原始图像(Moon)', (2, 2, 1), plt.gcf())
    show_img_hist(img_log, '对数变换', (2, 2, 3), plt.gcf())
    plt.tight_layout()
    plt.savefig(os.path.join(save_dir, 'results_log.png'), dpi=150, bbox_inches='tight')
    plt.close()

    # 6. Gamma correction (Power-law transformation)
    gamma = 0.5
    img_gamma_05 = np.power(img_camera.astype(float) / 255.0, gamma) * 255.0
    img_gamma_05 = np.uint8(img_gamma_05)

    gamma = 2.0
    img_gamma_20 = np.power(img_camera.astype(float) / 255.0, gamma) * 255.0
    img_gamma_20 = np.uint8(img_gamma_20)

    plt.figure(figsize=(12, 12))
    show_img_hist(img_camera, '原始图像', (3, 2, 1), plt.gcf())
    show_img_hist(img_gamma_05, 'Gamma变换 (gamma=0.5)', (3, 2, 3), plt.gcf())
    show_img_hist(img_gamma_20, 'Gamma变换 (gamma=2.0)', (3, 2, 5), plt.gcf())
    plt.tight_layout()
    plt.savefig(os.path.join(save_dir, 'results_gamma.png'), dpi=150, bbox_inches='tight')
    plt.close()

    # 7. Comprehensive comparison
    plt.figure(figsize=(15, 10))
    plt.subplot(2, 3, 1)
    plt.imshow(img_camera, cmap='gray', vmin=0, vmax=255)
    plt.title('原始图像 (Camera)')
    plt.axis('off')
    
    plt.subplot(2, 3, 2)
    plt.imshow(img_negative, cmap='gray', vmin=0, vmax=255)
    plt.title('灰度反转')
    plt.axis('off')

    plt.subplot(2, 3, 3)
    plt.imshow(img_stretch, cmap='gray', vmin=0, vmax=255)
    plt.title('灰度拉伸')
    plt.axis('off')

    plt.subplot(2, 3, 4)
    plt.imshow(img_moon, cmap='gray', vmin=0, vmax=255)
    plt.title('原始图像 (Moon)')
    plt.axis('off')

    plt.subplot(2, 3, 5)
    plt.imshow(img_log, cmap='gray', vmin=0, vmax=255)
    plt.title('对数变换')
    plt.axis('off')

    plt.subplot(2, 3, 6)
    plt.imshow(img_gamma_05, cmap='gray', vmin=0, vmax=255)
    plt.title('Gamma变换 (0.5)')
    plt.axis('off')
    
    plt.tight_layout()
    plt.savefig(os.path.join(save_dir, 'results.png'), dpi=150, bbox_inches='tight')
    plt.close()

if __name__ == '__main__':
    main()