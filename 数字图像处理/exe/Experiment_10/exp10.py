import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage import data
import os

# Set up matplotlib for Chinese fonts
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# Create output directory if it doesn't exist
output_dir = 'Experiment_10'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def save_fig(fig, filename):
    fig.savefig(os.path.join(output_dir, filename), dpi=150, bbox_inches='tight')
    plt.close(fig)

def add_gaussian_noise(image, mean=0, sigma=25):
    noise = np.random.normal(mean, sigma, image.shape)
    noisy_image = np.clip(image + noise, 0, 255).astype(np.uint8)
    return noisy_image

def apply_roberts(img):
    kernel_x = np.array([[0, 1], [-1, 0]], dtype=np.float32)
    kernel_y = np.array([[1, 0], [0, -1]], dtype=np.float32)
    
    rx = cv2.filter2D(img.astype(np.float32), -1, kernel_x)
    ry = cv2.filter2D(img.astype(np.float32), -1, kernel_y)
    
    grad_euclidean = np.sqrt(rx**2 + ry**2)
    grad_block = np.abs(rx) + np.abs(ry)
    
    # Binarization
    _, binary = cv2.threshold(grad_euclidean.astype(np.uint8), 30, 255, cv2.THRESH_BINARY)
    
    return rx, ry, grad_euclidean, grad_block, binary

def apply_prewitt(img):
    kernel_x = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]], dtype=np.float32)
    kernel_y = np.array([[-1, -1, -1], [0, 0, 0], [1, 1, 1]], dtype=np.float32)
    
    px = cv2.filter2D(img.astype(np.float32), -1, kernel_x)
    py = cv2.filter2D(img.astype(np.float32), -1, kernel_y)
    
    grad = np.sqrt(px**2 + py**2)
    _, binary = cv2.threshold(grad.astype(np.uint8), 50, 255, cv2.THRESH_BINARY)
    
    return px, py, grad, binary

def apply_sobel(img):
    sx = cv2.Sobel(img.astype(np.float32), -1, 1, 0, ksize=3)
    sy = cv2.Sobel(img.astype(np.float32), -1, 0, 1, ksize=3)
    
    grad = np.sqrt(sx**2 + sy**2)
    _, binary = cv2.threshold(grad.astype(np.uint8), 50, 255, cv2.THRESH_BINARY)
    
    return sx, sy, grad, binary

def apply_log(img):
    blur = cv2.GaussianBlur(img, (3, 3), 0)
    log = cv2.Laplacian(blur, cv2.CV_32F, ksize=3)
    log_abs = np.abs(log)
    _, binary = cv2.threshold(log_abs.astype(np.uint8), 30, 255, cv2.THRESH_BINARY)
    return log, log_abs, binary

def apply_canny(img):
    return cv2.Canny(img, 100, 200)

def plot_operator_results(img, x_edges, y_edges, grad, binary, name, filename):
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    axes = axes.ravel()
    
    axes[0].imshow(img, cmap='gray')
    axes[0].set_title('原始图像')
    
    axes[1].imshow(np.abs(x_edges), cmap='gray')
    axes[1].set_title(f'{name} 水平边缘')
    
    axes[2].imshow(np.abs(y_edges), cmap='gray')
    axes[2].set_title(f'{name} 垂直边缘')
    
    axes[3].imshow(grad, cmap='gray')
    axes[3].set_title(f'{name} 梯度模')
    
    axes[4].imshow(binary, cmap='gray')
    axes[4].set_title(f'{name} 二值化')
    
    axes[5].axis('off')
    
    for ax in axes[:5]:
        ax.axis('off')
        
    plt.tight_layout()
    save_fig(fig, filename)

def main():
    # Load image (指导书指定: room.tif)
    img_path = 'media/p038_img01.png'
    if os.path.exists(img_path):
        img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
    else:
        img = data.camera()
    
    # 1. Roberts
    rx, ry, grad_eucl, grad_block, binary_roberts = apply_roberts(img)
    
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    axes = axes.ravel()
    axes[0].imshow(img, cmap='gray'); axes[0].set_title('原始图像'); axes[0].axis('off')
    axes[1].imshow(np.abs(rx), cmap='gray'); axes[1].set_title('Roberts 水平边缘'); axes[1].axis('off')
    axes[2].imshow(np.abs(ry), cmap='gray'); axes[2].set_title('Roberts 垂直边缘'); axes[2].axis('off')
    axes[3].imshow(grad_eucl, cmap='gray'); axes[3].set_title('梯度模 (欧几里德距离)'); axes[3].axis('off')
    axes[4].imshow(grad_block, cmap='gray'); axes[4].set_title('梯度模 (街区距离)'); axes[4].axis('off')
    axes[5].imshow(binary_roberts, cmap='gray'); axes[5].set_title('二值化'); axes[5].axis('off')
    plt.tight_layout()
    save_fig(fig, 'results_roberts.png')
    
    # 2. Prewitt
    px, py, grad_prewitt, binary_prewitt = apply_prewitt(img)
    plot_operator_results(img, px, py, grad_prewitt, binary_prewitt, 'Prewitt', 'results_prewitt.png')
    
    # 3. Sobel
    sx, sy, grad_sobel, binary_sobel = apply_sobel(img)
    plot_operator_results(img, sx, sy, grad_sobel, binary_sobel, 'Sobel', 'results_sobel.png')
    
    # 4. LoG
    log, log_abs, binary_log = apply_log(img)
    fig, axes = plt.subplots(1, 4, figsize=(20, 5))
    axes[0].imshow(img, cmap='gray'); axes[0].set_title('原始图像'); axes[0].axis('off')
    axes[1].imshow(log, cmap='gray'); axes[1].set_title('LoG算子结果'); axes[1].axis('off')
    axes[2].imshow(log_abs, cmap='gray'); axes[2].set_title('LoG绝对值'); axes[2].axis('off')
    axes[3].imshow(binary_log, cmap='gray'); axes[3].set_title('LoG二值化'); axes[3].axis('off')
    plt.tight_layout()
    save_fig(fig, 'results_log.png')
    
    # 5. Canny
    canny = apply_canny(img)
    
    # All together
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    axes[0, 0].imshow(img, cmap='gray'); axes[0, 0].set_title('原始图像')
    axes[0, 1].imshow(binary_roberts, cmap='gray'); axes[0, 1].set_title('Roberts 二值化')
    axes[0, 2].imshow(binary_prewitt, cmap='gray'); axes[0, 2].set_title('Prewitt 二值化')
    axes[1, 0].imshow(binary_sobel, cmap='gray'); axes[1, 0].set_title('Sobel 二值化')
    axes[1, 1].imshow(binary_log, cmap='gray'); axes[1, 1].set_title('LoG 二值化')
    axes[1, 2].imshow(canny, cmap='gray'); axes[1, 2].set_title('Canny 边缘')
    for ax in axes.ravel():
        ax.axis('off')
    plt.tight_layout()
    save_fig(fig, 'results.png')
    
    # Noise test
    img_noise = add_gaussian_noise(img, sigma=20)
    
    _, _, _, _, n_binary_roberts = apply_roberts(img_noise)
    _, _, _, n_binary_prewitt = apply_prewitt(img_noise)
    _, _, _, n_binary_sobel = apply_sobel(img_noise)
    _, _, n_binary_log = apply_log(img_noise)
    n_canny = apply_canny(img_noise)
    
    # Noise individual
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    axes[0, 0].imshow(img_noise, cmap='gray'); axes[0, 0].set_title('噪声图像')
    axes[0, 1].imshow(n_binary_roberts, cmap='gray'); axes[0, 1].set_title('Roberts (噪声)')
    axes[0, 2].imshow(n_binary_prewitt, cmap='gray'); axes[0, 2].set_title('Prewitt (噪声)')
    axes[1, 0].imshow(n_binary_sobel, cmap='gray'); axes[1, 0].set_title('Sobel (噪声)')
    axes[1, 1].imshow(n_binary_log, cmap='gray'); axes[1, 1].set_title('LoG (噪声)')
    axes[1, 2].imshow(n_canny, cmap='gray'); axes[1, 2].set_title('Canny (噪声)')
    for ax in axes.ravel():
        ax.axis('off')
    plt.tight_layout()
    save_fig(fig, 'results_noise.png')
    save_fig(fig, 'results_comparison.png') # saving the same figure for results_comparison.png as required

if __name__ == '__main__':
    main()
