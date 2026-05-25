import cv2
import numpy as np
import os
import matplotlib.pyplot as plt
from PIL import Image

def get_image_info(img_path):
    """获取图像文件的详细信息（等效于MATLAB的imfinfo）"""
    img = Image.open(img_path)
    file_stats = os.stat(img_path)
    info = {
        '文件名': os.path.basename(img_path),
        '文件大小(bytes)': file_stats.st_size,
        '图像格式': img.format,
        '图像模式': img.mode,
        '图像尺寸(宽×高)': img.size,
        '色彩空间': img.mode,
    }
    img_cv = cv2.imread(img_path)
    if img_cv is not None:
        info['通道数'] = img_cv.shape[2] if len(img_cv.shape) == 3 else 1
        info['数据类型'] = str(img_cv.dtype)
        info['像素值范围'] = f'[{img_cv.min()}, {img_cv.max()}]'
    return info

def print_image_info(info, title=""):
    print(f"\n{'='*40}")
    if title:
        print(f"  {title}")
        print(f"{'='*40}")
    for key, value in info.items():
        print(f"  {key}: {value}")

def main():
    print('='*60)
    print('  实验一：MATLAB数字图像基本处理 (Python实现)')
    print('='*60)

    # 指导书指定图像路径（从media目录读取）
    flower_path = 'media/p008_img01.png'
    lenna_path  = 'media/p008_img03.png'
    camema_path = 'media/p008_img04.png'
    # 备用路径（如从Experiment_1目录直接运行）
    flower_local = 'Experiment_1/flower.tif'
    lenna_local  = 'Experiment_1/lenna.png'

    # ===== Step 1 & 2: 读取图像并显示基本信息（flower.tif）=====
    print('\n--- Step 1 & 2: 读取图像并显示基本信息 ---')
    img_path = flower_path if os.path.exists(flower_path) else (flower_local if os.path.exists(flower_local) else lenna_path)
    img = cv2.imread(img_path)

    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    file_stats = os.stat(img_path)
    print(f"图像文件: {os.path.basename(img_path)}")
    print(f"文件大小: {file_stats.st_size} bytes")
    print(f"图像形状: {img.shape}")
    print(f"图像数据类型: {img.dtype}")
    print(f"图像高度: {img.shape[0]} pixels")
    print(f"图像宽度: {img.shape[1]} pixels")
    print(f"通道数: {img.shape[2]}")

    # ===== Step 3: imfinfo 获取详细信息 =====
    print('\n--- Step 3: 获取图像文件详细信息 (imfinfo等效) ---')
    info = get_image_info(img_path)
    print_image_info(info, os.path.basename(img_path))

    # ===== Step 4: 压缩存储为JPEG（flower.jpg, quality=50）=====
    print('\n--- Step 4: 压缩存储为JPEG ---')
    jpg_path = 'Experiment_1/flower_compressed.jpg'
    cv2.imwrite(jpg_path, img, [int(cv2.IMWRITE_JPEG_QUALITY), 50])
    jpg_stats = os.stat(jpg_path)
    print(f"压缩后文件大小: {jpg_stats.st_size} bytes (质量=50)")
    print(f"压缩比: {file_stats.st_size / jpg_stats.st_size:.2f}x")

    # ===== Step 5: 另存为BMP格式（flower.bmp）=====
    print('\n--- Step 5: 另存为BMP格式 ---')
    bmp_path = 'Experiment_1/flower.bmp'
    cv2.imwrite(bmp_path, img)
    bmp_stats = os.stat(bmp_path)
    print(f"BMP文件大小: {bmp_stats.st_size} bytes")

    # ===== Step 6 & 7: 读取Lenna.jpg和camema.jpg并比较 =====
    print('\n--- Step 6 & 7: 读取Lenna.jpg和camema.jpg ---')
    lenna_actual = lenna_path if os.path.exists(lenna_path) else lenna_local
    camema_actual = camema_path if os.path.exists(camema_path) else 'Experiment_1/camema.jpg'

    img_lenna = cv2.imread(lenna_actual)
    img_camema = cv2.imread(camema_actual)

    print(f"\n图像1 ({os.path.basename(lenna_actual)}):")
    info1 = get_image_info(lenna_actual)
    print_image_info(info1)

    print(f"\n图像2 ({os.path.basename(camema_actual)}):")
    info2 = get_image_info(camema_actual)
    print_image_info(info2)

    # ===== Step 8: 灰度图转二值图（im2bw）=====
    print('\n--- Step 8: 灰度图转二值图 (im2bw等效) ---')
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, img_bw_otsu = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    _, img_bw_fixed = cv2.threshold(img_gray, 127, 255, cv2.THRESH_BINARY)

    bw_path = 'Experiment_1/flower_bw.png'
    cv2.imwrite(bw_path, img_bw_otsu)
    print(f"Otsu自动阈值: {_}")
    print(f"二值图像已保存: {bw_path}")

    # ===== Step 9: 绘制结果图 =====
    print('\n--- Step 9: 绘制结果图 ---')
    fig, axs = plt.subplots(2, 3, figsize=(18, 12))

    axs[0, 0].imshow(img_rgb)
    axs[0, 0].set_title(f'Original ({os.path.basename(img_path)})\n({file_stats.st_size} bytes)')
    axs[0, 0].axis('off')

    img_jpg_rgb = cv2.cvtColor(cv2.imread(jpg_path), cv2.COLOR_BGR2RGB)
    axs[0, 1].imshow(img_jpg_rgb)
    axs[0, 1].set_title(f'Compressed JPEG (Q=50)\n({jpg_stats.st_size} bytes)')
    axs[0, 1].axis('off')

    if img_lenna is not None:
        axs[0, 2].imshow(cv2.cvtColor(img_lenna, cv2.COLOR_BGR2RGB))
        axs[0, 2].set_title(f'Lenna\n({os.path.getsize(lenna_actual)} bytes)')
    else:
        axs[0, 2].text(0.5, 0.5, 'Lenna not found', ha='center', va='center')
    axs[0, 2].axis('off')

    axs[1, 0].imshow(img_gray, cmap='gray')
    axs[1, 0].set_title('Grayscale Image')
    axs[1, 0].axis('off')

    axs[1, 1].imshow(img_bw_otsu, cmap='gray')
    axs[1, 1].set_title('Binary (Otsu Threshold)')
    axs[1, 1].axis('off')

    axs[1, 2].imshow(img_bw_fixed, cmap='gray')
    axs[1, 2].set_title('Binary (Fixed Threshold=127)')
    axs[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_1/results.png', dpi=150)
    print("结果图已保存: Experiment_1/results.png")

    # ===== 文件大小对比 =====
    print('\n--- 文件格式大小对比 ---')
    print(f"  PNG:  {file_stats.st_size:>10} bytes")
    print(f"  JPEG (Q=50): {jpg_stats.st_size:>10} bytes")
    print(f"  BMP:  {bmp_stats.st_size:>10} bytes")
    print(f"  压缩比 (PNG/JPEG): {file_stats.st_size / jpg_stats.st_size:.2f}x")

    print("\n实验一完成。")

if __name__ == '__main__':
    main()
