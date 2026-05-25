import cv2
import numpy as np
import matplotlib.pyplot as plt
import os
import urllib.request

def download_plane2():
    """下载Plane2.jpg标准测试图像"""
    save_path = 'Experiment_5/Plane2.jpg'
    if os.path.exists(save_path):
        print(f"  Plane2.jpg 已存在")
        return save_path

    # 尝试从多个源下载
    urls = [
        ('https://www.ece.rice.edu/~wakin/images/Plane2.jpg', 'Plane2.jpg'),
        ('https://sipi.usc.edu/database/preview/misc/4.2.03.png', 'Plane2_download.png'),
    ]

    for url, filename in urls:
        try:
            print(f"  尝试下载: {url}")
            req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
            save_path = f'Experiment_5/{filename}'
            with urllib.request.urlopen(req, timeout=10) as response:
                with open(save_path, 'wb') as f:
                    f.write(response.read())
            # 如果下载的是PNG，转换为JPG（指导书要求.jpg）
            if filename.endswith('.png'):
                png_img = cv2.imread(save_path, cv2.IMREAD_GRAYSCALE)
                if png_img is not None:
                    jpg_path = 'Experiment_5/Plane2.jpg'
                    cv2.imwrite(jpg_path, png_img)
                    save_path = jpg_path
            print(f"  下载成功: {save_path}")
            return save_path
        except Exception as e:
            print(f"  下载失败: {e}")

    # 如果下载失败，使用lenna.png作为替代
    print("  Plane2.jpg 下载失败，使用 lenna.png 作为替代")
    return 'Experiment_5/lenna.png'

def main():
    print('='*60)
    print('  实验五：形态学图像处理 (Python实现)')
    print('='*60)

    # ===== Step 1: 调入并显示图像 =====
    print('\n--- Step 1: 调入并显示图像 ---')
    img_path = download_plane2()
    img_gray = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)

    if img_gray is None:
        print(f"  错误: 无法读取图像 {img_path}")
        return

    print(f"  图像尺寸: {img_gray.shape}")
    print(f"  灰度范围: [{img_gray.min()}, {img_gray.max()}]")

    # ===== Step 2: 选取合适的阈值，得到二值化图像 =====
    print('\n--- Step 2: 二值化处理 ---')
    # 使用Otsu自动阈值（等效于MATLAB的graythresh + im2bw，正向二值化）
    threshold_val, img_bin = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    print(f"  Otsu自动阈值: {threshold_val}")

    # 也可以使用graythresh等效方法
    # level = cv2.threshold(img_gray, 0, 255, cv2.THRESH_OTSU)[0] / 255.0

    # ===== Step 3: 设置结构元素 =====
    print('\n--- Step 3: 设置结构元素 ---')

    # 膨胀结构元素: 3×3方形 (等效于MATLAB的strel('square',3))
    kernel_dilate = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    print(f"  膨胀结构元素 (3×3方形):\n{kernel_dilate}")

    # 腐蚀结构元素: 5×5对角矩阵 (等效于MATLAB的strel('arbitrary',eye(5)))
    kernel_erode = np.eye(5, dtype=np.uint8)
    print(f"  腐蚀结构元素 (5×5对角):\n{kernel_erode}")

    # 通用结构元素: 5×5椭圆形
    kernel_ellipse = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    print(f"  椭圆结构元素 (5×5):\n{kernel_ellipse}")

    # ===== Step 4: 腐蚀运算 =====
    print('\n--- Step 4: 腐蚀运算 ---')
    img_erode = cv2.erode(img_bin, kernel_erode, iterations=1)
    print("  腐蚀完成 (使用5×5对角结构元素)")

    # ===== Step 5: 膨胀运算 =====
    print('\n--- Step 5: 膨胀运算 ---')
    img_dilate = cv2.dilate(img_bin, kernel_dilate, iterations=1)
    print("  膨胀完成 (使用3×3方形结构元素)")

    # ===== Step 6: 开运算 =====
    print('\n--- Step 6: 开运算 ---')
    img_open = cv2.morphologyEx(img_bin, cv2.MORPH_OPEN, kernel_ellipse)
    print("  开运算完成 (先腐蚀后膨胀，使用5×5椭圆结构元素)")

    # ===== Step 7: 闭运算 =====
    print('\n--- Step 7: 闭运算 ---')
    img_close = cv2.morphologyEx(img_bin, cv2.MORPH_CLOSE, kernel_ellipse)
    print("  闭运算完成 (先膨胀后腐蚀，使用5×5椭圆结构元素)")

    # ===== Step 8: 绘制结果图 =====
    print('\n--- Step 8: 绘制结果图 ---')

    # 图1: 原图、二值化、腐蚀、膨胀
    fig1, axs1 = plt.subplots(2, 3, figsize=(15, 10))
    fig1.suptitle('形态学图像处理结果', fontsize=16)

    axs1[0, 0].imshow(img_gray, cmap='gray')
    axs1[0, 0].set_title('Original Grayscale')
    axs1[0, 0].axis('off')

    axs1[0, 1].imshow(img_bin, cmap='gray')
    axs1[0, 1].set_title(f'Binary (Otsu, T={threshold_val:.0f})')
    axs1[0, 1].axis('off')

    axs1[0, 2].imshow(img_erode, cmap='gray')
    axs1[0, 2].set_title('Erosion (5×5 diagonal)')
    axs1[0, 2].axis('off')

    axs1[1, 0].imshow(img_dilate, cmap='gray')
    axs1[1, 0].set_title('Dilation (3×3 square)')
    axs1[1, 0].axis('off')

    axs1[1, 1].imshow(img_open, cmap='gray')
    axs1[1, 1].set_title('Opening (5×5 ellipse)')
    axs1[1, 1].axis('off')

    axs1[1, 2].imshow(img_close, cmap='gray')
    axs1[1, 2].set_title('Closing (5×5 ellipse)')
    axs1[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_5/results.png', dpi=150)
    print("  结果图已保存: results.png")

    # 图2: 对比分析
    fig2, axs2 = plt.subplots(2, 2, figsize=(12, 12))
    fig2.suptitle('形态学运算效果对比', fontsize=16)

    # 原始二值图
    axs2[0, 0].imshow(img_bin, cmap='gray')
    axs2[0, 0].set_title('Binary Original')
    axs2[0, 0].axis('off')

    # 腐蚀后膨胀（开运算的效果）
    axs2[0, 1].imshow(img_open, cmap='gray')
    axs2[0, 1].set_title('Opening (Erode→Dilate)')
    axs2[0, 1].axis('off')

    # 膨胀后腐蚀（闭运算的效果）
    axs2[1, 0].imshow(img_close, cmap='gray')
    axs2[1, 0].set_title('Closing (Dilate→Erode)')
    axs2[1, 0].axis('off')

    # 开运算后再闭运算
    img_open_close = cv2.morphologyEx(img_open, cv2.MORPH_CLOSE, kernel_ellipse)
    axs2[1, 1].imshow(img_open_close, cmap='gray')
    axs2[1, 1].set_title('Opening then Closing')
    axs2[1, 1].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_5/results_comparison.png', dpi=150)
    print("  对比图已保存: results_comparison.png")

    print("\n实验五完成。")

if __name__ == '__main__':
    main()
