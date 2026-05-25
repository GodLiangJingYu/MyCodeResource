import cv2
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

def main():
    # 指导书指定: flower1.tif
    flower_path = 'media/p008_img01.png'
    img_bgr = cv2.imread(flower_path)
    if img_bgr is None:
        img_bgr = cv2.imread('Experiment_4/lenna.png')
    # 如果是灰度图（PDF提取），转为3通道以支持RGB分析
    if len(img_bgr.shape) == 2:
        img_bgr = cv2.cvtColor(img_bgr, cv2.COLOR_GRAY2BGR)
    img_rgb = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2RGB)

    print('='*60)
    print('  实验四：彩色图像处理 (Python实现)')
    print('='*60)

    # ===== (1) 彩色图像分析 =====
    print('\n--- (1) 彩色图像分析：拆分RGB分量 ---')
    R, G, B = cv2.split(img_rgb)
    print(f"  R通道: shape={R.shape}, 范围=[{R.min()}, {R.max()}], 均值={R.mean():.1f}")
    print(f"  G通道: shape={G.shape}, 范围=[{G.min()}, {G.max()}], 均值={G.mean():.1f}")
    print(f"  B通道: shape={B.shape}, 范围=[{B.min()}, {B.max()}], 均值={B.mean():.1f}")

    # 转换到HSV空间分析
    img_hsv = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2HSV)
    H, S, V = cv2.split(img_hsv)
    print(f"  H(色调): shape={H.shape}, 范围=[{H.min()}, {H.max()}], 均值={H.mean():.1f}")
    print(f"  S(饱和度): shape={S.shape}, 范围=[{S.min()}, {S.max()}], 均值={S.mean():.1f}")
    print(f"  V(明度): shape={V.shape}, 范围=[{V.min()}, {V.max()}], 均值={V.mean():.1f}")

    # ===== (2) 彩色图像直方图均衡 =====
    print('\n--- (2) R/G/B分量直方图均衡 ---')

    # 对R/G/B三个通道分别进行直方图均衡
    R_eq = cv2.equalizeHist(R)
    G_eq = cv2.equalizeHist(G)
    B_eq = cv2.equalizeHist(B)

    # 合并均衡后的通道
    img_rgb_eq = cv2.merge([R_eq, G_eq, B_eq])

    # 对V通道进行直方图均衡（HSV空间）
    V_eq = cv2.equalizeHist(V)
    img_hsv_eq = cv2.merge([H, S, V_eq])
    img_rgb_hsv_eq = cv2.cvtColor(img_hsv_eq, cv2.COLOR_HSV2RGB)

    print("  R/G/B通道分别均衡化完成")
    print("  HSV空间V通道均衡化完成")

    # ===== (3) 假彩色处理 =====
    print('\n--- (3) 假彩色处理 ---')

    # 先计算灰度图（伪彩色处理等后续步骤会用到）
    img_gray = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2GRAY)

    # 加载多波长图像（从media目录，由generate_multispectral.py生成）
    # v1_red/v1_green/v1_blue: 模拟可见光R/G/B分量（从flower通道提取）
    # infer_near/infer_mid: 模拟近红外/中红外（从flower灰度图处理得到）
    vl_red_path   = 'media/v1_red.jpg'
    vl_green_path = 'media/v1_green.jpg'
    vl_blue_path  = 'media/v1_blue.jpg'
    infer_near_path = 'media/infer_near.jpg'
    infer_mid_path  = 'media/infer_mid.jpg'

    f1 = cv2.imread(vl_red_path, cv2.IMREAD_GRAYSCALE)
    f2 = cv2.imread(vl_green_path, cv2.IMREAD_GRAYSCALE)
    f3 = cv2.imread(vl_blue_path, cv2.IMREAD_GRAYSCALE)
    f4_near = cv2.imread(infer_near_path, cv2.IMREAD_GRAYSCALE)
    f5_mid  = cv2.imread(infer_mid_path, cv2.IMREAD_GRAYSCALE)

    # 若图片存在则使用，否则回退到模拟
    if all(x is not None for x in [f1, f2, f3, f4_near, f5_mid]):
        print("  已加载多波长图像：vl_red, vl_green, vl_blue, infer_near, infer_mid")
        true_color = cv2.merge([f1, f2, f3])  # 可见光RGB合成
        near_infrared = f4_near
        mid_infrared  = f5_mid
    else:
        print("  多波长图像不存在，使用模拟图像")
        img_gray = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2GRAY)
        true_color = cv2.merge([R, G, B])
        near_infrared = cv2.equalizeHist(cv2.GaussianBlur(img_gray, (5, 5), 0))
        mid_infrared = cv2.equalizeHist(np.clip(np.abs(cv2.Laplacian(img_gray, cv2.CV_64F)), 0, 255).astype(np.uint8))

    # 可见光RGB彩色图像（真彩色）
    print(f"  真彩色合成: shape={true_color.shape}")

    # 假彩色合成1：近红外替换R分量
    false_color_near = cv2.merge([near_infrared, f2 if f2 is not None else G, f3 if f3 is not None else B])

    # 假彩色合成2：中红外替换R分量
    false_color_mid = cv2.merge([mid_infrared, f2 if f2 is not None else G, f3 if f3 is not None else B])

    print("  近红外假彩色合成完成 (近红外→R, G→G, B→B)")
    print("  中红外假彩色合成完成 (中红外→R, G→G, B→B)")

    # ===== (4) 伪彩色处理：灰度切片 =====
    print('\n--- (4) 伪彩色处理：灰度切片 ---')

    # 灰度切片处理（等效于MATLAB的imadjust提取特定灰度级后再colormap）
    # 指导书方法：用imadjust提取灰度在16-128和128-256范围的像素，分别用hot/cool彩色化

    # 灰度归一化
    img_normalized = img_gray.astype(np.float64) / 255.0

    # 方法1：整体伪彩色（直接应用colormap）
    hot_cmap = cm.hot
    img_hot = hot_cmap(img_normalized)
    img_hot = (img_hot[:, :, :3] * 255).astype(np.uint8)

    cool_cmap = cm.cool
    img_cool = cool_cmap(img_normalized)
    img_cool = (img_cool[:, :, :3] * 255).astype(np.uint8)

    # 方法2：imadjust等效的灰度切片+伪彩色（指导书要求）
    # cut_1 = imadjust(f,[0.0925 0.5],[0.0925 0.5]); 提取灰度在16-128之间的像素
    # cut_2 = imadjust(f,[0.5 1],[0.5 1]); 提取灰度在128-256之间的像素
    # figure,imshow(cut_1),colormap(hot)
    # figure,imshow(cut_2),colormap(cool)

    # Python等效实现：先imadjust再colormap
    def imadjust(img_norm, low_in, high_in, low_out, high_out):
        """等效于MATLAB的imadjust：将[low_in, high_in]范围映射到[low_out, high_out]"""
        result = np.copy(img_norm)
        mask = (img_norm >= low_in) & (img_norm <= high_in)
        result[mask] = low_out + (high_out - low_out) * (img_norm[mask] - low_in) / (high_in - low_in)
        result[img_norm < low_in] = 0
        result[img_norm > high_in] = 1
        return result

    # 灰度切片1：提取灰度在[0.0925, 0.5]范围内（约16-128级）
    cut_1 = imadjust(img_normalized, 0.0925, 0.5, 0.0925, 0.5)
    # 灰度切片2：提取灰度在[0.5, 1.0]范围内（约128-256级）
    cut_2 = imadjust(img_normalized, 0.5, 1.0, 0.5, 1.0)

    # 分别应用hot和cool colormap
    img_cut1_hot = hot_cmap(cut_1)
    img_cut1_hot = (img_cut1_hot[:, :, :3] * 255).astype(np.uint8)

    img_cut2_cool = cool_cmap(cut_2)
    img_cut2_cool = (img_cut2_cool[:, :, :3] * 255).astype(np.uint8)

    # 合并切片伪彩色图像
    img_sliced = np.zeros((*img_gray.shape, 3), dtype=np.uint8)
    mask_low = (img_normalized >= 0.0925) & (img_normalized < 0.5)
    mask_high = img_normalized >= 0.5
    img_sliced[mask_low] = img_cut1_hot[mask_low]
    img_sliced[mask_high] = img_cut2_cool[mask_high]

    print("  imadjust等效灰度切片完成: [0.0925,0.5)→hot, [0.5,1.0)→cool")
    print("  hot/cool模式伪彩色处理完成")

    # ===== (5) 彩色变换（选做）=====
    print('\n--- (5) 彩色变换（选做）---')

    # 使用正弦函数进行彩色变换
    x = np.linspace(0, 2 * np.pi, 256)
    # R = -sin(x), G = -cos(x), B = sin(x)
    transform_R = (-np.sin(x) + 1) / 2  # 归一化到[0,1]
    transform_G = (-np.cos(x) + 1) / 2
    transform_B = (np.sin(x) + 1) / 2

    # 应用变换
    img_transformed = np.zeros_like(img_rgb, dtype=np.float64)
    img_transformed[:, :, 0] = transform_R[img_gray]
    img_transformed[:, :, 1] = transform_G[img_gray]
    img_transformed[:, :, 2] = transform_B[img_gray]
    img_transformed = (img_transformed * 255).astype(np.uint8)

    # 互换R和G
    img_transformed_swap = np.zeros_like(img_rgb, dtype=np.float64)
    img_transformed_swap[:, :, 0] = transform_G[img_gray]  # R用G的变换
    img_transformed_swap[:, :, 1] = transform_R[img_gray]  # G用R的变换
    img_transformed_swap[:, :, 2] = transform_B[img_gray]
    img_transformed_swap = (img_transformed_swap * 255).astype(np.uint8)

    print("  正弦函数彩色变换完成")
    print("  R=-sin, G=-cos, B=sin")
    print("  R与G互换后的变换完成")

    # ===== (6) 绘制结果图 =====
    print('\n--- (6) 绘制结果图 ---')

    # 图1: RGB分量与直方图
    fig1, axs1 = plt.subplots(3, 4, figsize=(20, 15))
    fig1.suptitle('彩色图像分析与RGB直方图均衡', fontsize=16)

    # 原图
    axs1[0, 0].imshow(img_rgb)
    axs1[0, 0].set_title('Original RGB')
    axs1[0, 0].axis('off')

    axs1[0, 1].imshow(R, cmap='Reds')
    axs1[0, 1].set_title('Red Channel')
    axs1[0, 1].axis('off')

    axs1[0, 2].imshow(G, cmap='Greens')
    axs1[0, 2].set_title('Green Channel')
    axs1[0, 2].axis('off')

    axs1[0, 3].imshow(B, cmap='Blues')
    axs1[0, 3].set_title('Blue Channel')
    axs1[0, 3].axis('off')

    # 均衡后的通道
    axs1[1, 0].imshow(img_rgb_eq)
    axs1[1, 0].set_title('RGB Histogram Eq')
    axs1[1, 0].axis('off')

    axs1[1, 1].imshow(R_eq, cmap='Reds')
    axs1[1, 1].set_title('R Equalized')
    axs1[1, 1].axis('off')

    axs1[1, 2].imshow(G_eq, cmap='Greens')
    axs1[1, 2].set_title('G Equalized')
    axs1[1, 2].axis('off')

    axs1[1, 3].imshow(B_eq, cmap='Blues')
    axs1[1, 3].set_title('B Equalized')
    axs1[1, 3].axis('off')

    # 直方图对比
    axs1[2, 0].hist(R.ravel(), 256, [0, 256], color='red', alpha=0.5, label='Original')
    axs1[2, 0].hist(R_eq.ravel(), 256, [0, 256], color='darkred', alpha=0.5, label='Equalized')
    axs1[2, 0].set_title('R Histogram')
    axs1[2, 0].legend()

    axs1[2, 1].hist(G.ravel(), 256, [0, 256], color='green', alpha=0.5, label='Original')
    axs1[2, 1].hist(G_eq.ravel(), 256, [0, 256], color='darkgreen', alpha=0.5, label='Equalized')
    axs1[2, 1].set_title('G Histogram')
    axs1[2, 1].legend()

    axs1[2, 2].hist(B.ravel(), 256, [0, 256], color='blue', alpha=0.5, label='Original')
    axs1[2, 2].hist(B_eq.ravel(), 256, [0, 256], color='darkblue', alpha=0.5, label='Equalized')
    axs1[2, 2].set_title('B Histogram')
    axs1[2, 2].legend()

    axs1[2, 3].imshow(img_rgb_hsv_eq)
    axs1[2, 3].set_title('HSV V-Channel Eq')
    axs1[2, 3].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_4/results_rgb.png', dpi=150)
    print("  RGB直方图均衡结果已保存: results_rgb.png")

    # 图2: 假彩色处理
    fig2, axs2 = plt.subplots(2, 3, figsize=(18, 12))
    fig2.suptitle('假彩色处理', fontsize=16)

    # 显示可见光三个通道
    if f1 is not None:
        axs2[0, 0].imshow(f1, cmap='Reds')
        axs2[0, 0].set_title('vl_red (红光通道)')
    else:
        axs2[0, 0].imshow(img_rgb)
        axs2[0, 0].set_title('Original RGB')
    axs2[0, 0].axis('off')

    axs2[0, 1].imshow(near_infrared, cmap='gray')
    axs2[0, 1].set_title('近红外 (infer_near)')
    axs2[0, 1].axis('off')

    axs2[0, 2].imshow(mid_infrared, cmap='gray')
    axs2[0, 2].set_title('中红外 (infer_mid)')
    axs2[0, 2].axis('off')

    axs2[1, 0].imshow(true_color)
    axs2[1, 0].set_title('可见光真彩色 (vl_red+green+blue)')
    axs2[1, 0].axis('off')

    axs2[1, 1].imshow(false_color_near)
    axs2[1, 1].set_title('假彩色 (近红外→R, green→G, blue→B)')
    axs2[1, 1].axis('off')

    axs2[1, 2].imshow(false_color_mid)
    axs2[1, 2].set_title('False Color (Mid-IR,R→G,B)')
    axs2[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_4/results_falsecolor.png', dpi=150)
    print("  假彩色处理结果已保存: results_falsecolor.png")

    # 图3: 伪彩色处理
    fig3, axs3 = plt.subplots(2, 4, figsize=(24, 12))
    fig3.suptitle('伪彩色处理与彩色变换', fontsize=16)

    axs3[0, 0].imshow(img_gray, cmap='gray')
    axs3[0, 0].set_title('Grayscale Image')
    axs3[0, 0].axis('off')

    axs3[0, 1].imshow(img_hot)
    axs3[0, 1].set_title('Hot Colormap (all)')
    axs3[0, 1].axis('off')

    axs3[0, 2].imshow(img_cool)
    axs3[0, 2].set_title('Cool Colormap (all)')
    axs3[0, 2].axis('off')

    axs3[0, 3].imshow(img_cut1_hot)
    axs3[0, 3].set_title('imadjust [0.0925,0.5]\n+ Hot Colormap')
    axs3[0, 3].axis('off')

    axs3[1, 0].imshow(img_cut2_cool)
    axs3[1, 0].set_title('imadjust [0.5,1.0]\n+ Cool Colormap')
    axs3[1, 0].axis('off')

    axs3[1, 1].imshow(img_sliced)
    axs3[1, 1].set_title('Gray Slicing Combined\n([0.0925,0.5)→Hot, [0.5,1.0)→Cool)')
    axs3[1, 1].axis('off')

    axs3[1, 2].imshow(img_transformed)
    axs3[1, 2].set_title('Sin Transform\n(R=-sin, G=-cos, B=sin)')
    axs3[1, 2].axis('off')

    axs3[1, 3].imshow(img_transformed_swap)
    axs3[1, 3].set_title('Sin Transform (R↔G swapped)')
    axs3[1, 3].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_4/results_pseudocolor.png', dpi=150)
    print("  伪彩色处理结果已保存: results_pseudocolor.png")

    # 图4: 综合结果
    fig4, axs4 = plt.subplots(3, 4, figsize=(20, 15))
    fig4.suptitle('彩色图像处理综合结果', fontsize=16)

    # 第一行: RGB分析
    axs4[0, 0].imshow(img_rgb)
    axs4[0, 0].set_title('Original')
    axs4[0, 0].axis('off')

    axs4[0, 1].imshow(R, cmap='Reds')
    axs4[0, 1].set_title('R Channel')
    axs4[0, 1].axis('off')

    axs4[0, 2].imshow(G, cmap='Greens')
    axs4[0, 2].set_title('G Channel')
    axs4[0, 2].axis('off')

    axs4[0, 3].imshow(B, cmap='Blues')
    axs4[0, 3].set_title('B Channel')
    axs4[0, 3].axis('off')

    # 第二行: 增强与假彩色
    axs4[1, 0].imshow(img_rgb_eq)
    axs4[1, 0].set_title('RGB Eq')
    axs4[1, 0].axis('off')

    axs4[1, 1].imshow(img_rgb_hsv_eq)
    axs4[1, 1].set_title('HSV V-Eq')
    axs4[1, 1].axis('off')

    axs4[1, 2].imshow(false_color_near)
    axs4[1, 2].set_title('False Color (Near-IR)')
    axs4[1, 2].axis('off')

    axs4[1, 3].imshow(false_color_mid)
    axs4[1, 3].set_title('False Color (Mid-IR)')
    axs4[1, 3].axis('off')

    # 第三行: 伪彩色与变换
    axs4[2, 0].imshow(img_cut1_hot)
    axs4[2, 0].set_title('imadjust+Hot')
    axs4[2, 0].axis('off')

    axs4[2, 1].imshow(img_cut2_cool)
    axs4[2, 1].set_title('imadjust+Cool')
    axs4[2, 1].axis('off')

    axs4[2, 2].imshow(img_transformed)
    axs4[2, 2].set_title('Sin Transform')
    axs4[2, 2].axis('off')

    axs4[2, 3].imshow(img_transformed_swap)
    axs4[2, 3].set_title('Sin (R↔G)')
    axs4[2, 3].axis('off')

    plt.tight_layout()
    plt.savefig('Experiment_4/results.png', dpi=150)
    print("  综合结果图已保存: results.png")

    print("\n实验四完成。")

if __name__ == '__main__':
    main()
