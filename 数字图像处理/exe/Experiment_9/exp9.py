import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage import data
import os

# Set up matplotlib for Chinese fonts
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

output_dir = 'Experiment_9'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def save_fig(fig, filename):
    fig.savefig(os.path.join(output_dir, filename), dpi=150, bbox_inches='tight')
    plt.close(fig)

def main():
    # 1. Load image (指导书指定: number.tif)
    img_path = 'media/p037_img01.png'
    if os.path.exists(img_path):
        img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
    else:
        img = data.camera()

    # 2. 2D FFT
    f = np.fft.fft2(img)

    # 3. Shift zero-frequency to center
    fshift = np.fft.fftshift(f)

    # Magnitude and Phase
    magnitude = np.abs(fshift)
    phase = np.angle(fshift)

    magnitude_spectrum = np.log(1 + magnitude)

    # 4. Reconstruct using ONLY magnitude (phase = 0)
    # Phase = 0 means A * exp(i * 0) = A
    f_mag_only = np.fft.ifftshift(magnitude)
    img_mag_only = np.fft.ifft2(f_mag_only)
    img_mag_only = np.abs(img_mag_only)

    # 5. Reconstruct using ONLY phase (magnitude = 1)
    # Magnitude = 1 means 1 * exp(i * phase)
    f_phase_only = np.fft.ifftshift(np.exp(1j * phase))
    img_phase_only = np.fft.ifft2(f_phase_only)
    img_phase_only = np.abs(img_phase_only)

    # Output Figures
    # results_spectrum.png
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    axes[0].imshow(img, cmap='gray')
    axes[0].set_title('原图')
    axes[0].axis('off')

    axes[1].imshow(magnitude_spectrum, cmap='gray')
    axes[1].set_title('幅度谱')
    axes[1].axis('off')

    axes[2].imshow(phase, cmap='gray')
    axes[2].set_title('相位谱')
    axes[2].axis('off')
    save_fig(fig, 'results_spectrum.png')

    # results_reconstruction.png
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    axes[0].imshow(img, cmap='gray')
    axes[0].set_title('原图')
    axes[0].axis('off')

    axes[1].imshow(img_mag_only, cmap='gray')
    axes[1].set_title('仅含幅度的重构图像')
    axes[1].axis('off')

    axes[2].imshow(img_phase_only, cmap='gray')
    axes[2].set_title('仅含相位的重构图像')
    axes[2].axis('off')
    save_fig(fig, 'results_reconstruction.png')

    # results.png (Combined summary)
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    axes[0, 0].imshow(img, cmap='gray')
    axes[0, 0].set_title('原图')
    axes[0, 0].axis('off')

    axes[0, 1].imshow(magnitude_spectrum, cmap='gray')
    axes[0, 1].set_title('幅度谱')
    axes[0, 1].axis('off')

    axes[0, 2].imshow(phase, cmap='gray')
    axes[0, 2].set_title('相位谱')
    axes[0, 2].axis('off')

    axes[1, 0].imshow(img, cmap='gray')
    axes[1, 0].set_title('原图')
    axes[1, 0].axis('off')

    axes[1, 1].imshow(img_mag_only, cmap='gray')
    axes[1, 1].set_title('仅含幅度的重构图像')
    axes[1, 1].axis('off')

    axes[1, 2].imshow(img_phase_only, cmap='gray')
    axes[1, 2].set_title('仅含相位的重构图像')
    axes[1, 2].axis('off')
    plt.tight_layout()
    save_fig(fig, 'results.png')

if __name__ == '__main__':
    main()