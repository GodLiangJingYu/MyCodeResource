import numpy as np
import cv2
import matplotlib.pyplot as plt

def imread_chinese(path):
    """读取中文路径的图片"""
    import os
    img = cv2.imdecode(np.frombuffer(open(os.path.abspath(path), 'rb').read(), np.uint8), -1)
    return img

def compute_sift_keypoints(image):
    """使用SIFT算法计算关键点和描述子"""
    # 转换为灰度图
    if len(image.shape) == 3:
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    else:
        gray = image.copy()

    # 创建SIFT检测器
    sift = cv2.SIFT_create()

    # 检测关键点和计算描述子
    keypoints, descriptors = sift.detectAndCompute(gray, None)

    return keypoints, descriptors

def match_features(descriptors1, descriptors2, ratio_threshold=0.75):
    """使用BFMatcher进行特征匹配"""
    # 创建BFMatcher对象，使用NORM_L2距离（SIFT的默认距离度量）
    bf = cv2.BFMatcher(cv2.NORM_L2, crossCheck=False)

    # 进行KNN匹配，k=2用于应用比率测试
    matches = bf.knnMatch(descriptors1, descriptors2, k=2)

    # 应用比率测试（Lowe's ratio test）
    good_matches = []
    for m, n in matches:
        if m.distance < ratio_threshold * n.distance:
            good_matches.append(m)

    return good_matches, matches

def visualize_matches(image1, keypoints1, image2, keypoints2, matches, title="SIFT特征匹配"):
    """可视化特征匹配结果"""
    # 绘制匹配结果
    result = cv2.drawMatches(image1, keypoints1, image2, keypoints2, matches, None,
                           flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

    return result

def task2():
    """任务二：SIFT算法图像匹配"""
    # 读取两幅山脉图片
    image1_path = "山脉图1.png"
    image2_path = "山脉图2.png"

    image1 = imread_chinese(image1_path)
    image2 = imread_chinese(image2_path)

    image1_rgb = cv2.cvtColor(image1, cv2.COLOR_BGR2RGB)
    image2_rgb = cv2.cvtColor(image2, cv2.COLOR_BGR2RGB)

    # 提取SIFT关键点和描述子
    print("正在提取SIFT关键点和描述子...")
    keypoints1, descriptors1 = compute_sift_keypoints(image1)
    keypoints2, descriptors2 = compute_sift_keypoints(image2)

    print(f"图片1检测到 {len(keypoints1)} 个关键点")
    print(f"图片2检测到 {len(keypoints2)} 个关键点")

    # 设置不同的比率阈值进行对比
    ratio_thresholds = [0.7, 0.75, 0.8, 0.85]

    fig, axes = plt.subplots(2, 2, figsize=(20, 15))
    fig.suptitle('SIFT特征匹配不同参数对比', fontsize=16, fontproperties='SimHei')

    for idx, ratio in enumerate(ratio_thresholds):
        # 进行特征匹配
        good_matches, all_matches = match_features(descriptors1, descriptors2,
                                                   ratio_threshold=ratio)

        # 可视化匹配结果
        result = visualize_matches(image1_rgb, keypoints1,
                                 image2_rgb, keypoints2, good_matches[:100])

        row = idx // 2
        col = idx % 2

        axes[row, col].imshow(result)
        axes[row, col].set_title(f'比率阈值={ratio}\n匹配数量={len(good_matches)}',
                                fontproperties='SimHei')
        axes[row, col].axis('off')

    plt.tight_layout()
    plt.savefig('task2_result.png', dpi=300, bbox_inches='tight')
    plt.close()

    # 保存单幅匹配结果（高质量）
    good_matches, all_matches = match_features(descriptors1, descriptors2,
                                               ratio_threshold=0.75)
    result = visualize_matches(image1_rgb, keypoints1,
                             image2_rgb, keypoints2, good_matches[:50])
    plt.figure(figsize=(20, 10))
    plt.imshow(result)
    plt.title(f'SIFT特征匹配结果 (比率阈值=0.75, 匹配数量={len(good_matches)})',
              fontproperties='SimHei', fontsize=14)
    plt.axis('off')
    plt.tight_layout()
    plt.savefig('task2_single_match.png', dpi=300, bbox_inches='tight')
    plt.close()

    print(f"\n任务二完成！")
    print(f"比率阈值=0.70, 匹配数量={len(match_features(descriptors1, descriptors2, 0.70)[0])}")
    print(f"比率阈值=0.75, 匹配数量={len(match_features(descriptors1, descriptors2, 0.75)[0])}")
    print(f"比率阈值=0.80, 匹配数量={len(match_features(descriptors1, descriptors2, 0.80)[0])}")
    print(f"比率阈值=0.85, 匹配数量={len(match_features(descriptors1, descriptors2, 0.85)[0])}")
    print(f"结果已保存至 task2_result.png 和 task2_single_match.png")

    # 可视化关键点
    visualize_keypoints(image1, keypoints1, image2, keypoints2)

def visualize_keypoints(image1, keypoints1, image2, keypoints2):
    """可视化SIFT关键点"""
    image1_rgb = cv2.cvtColor(image1, cv2.COLOR_BGR2RGB)
    image2_rgb = cv2.cvtColor(image2, cv2.COLOR_BGR2RGB)

    image1_kp = cv2.drawKeypoints(image1_rgb, keypoints1, None,
                                   flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    image2_kp = cv2.drawKeypoints(image2_rgb, keypoints2, None,
                                   flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    fig, axes = plt.subplots(1, 2, figsize=(16, 8))
    fig.suptitle('SIFT关键点检测结果', fontsize=16, fontproperties='SimHei')

    axes[0].imshow(image1_kp)
    axes[0].set_title(f'山脉图1 - {len(keypoints1)} 个关键点', fontproperties='SimHei')
    axes[0].axis('off')

    axes[1].imshow(image2_kp)
    axes[1].set_title(f'山脉图2 - {len(keypoints2)} 个关键点', fontproperties='SimHei')
    axes[1].axis('off')

    plt.tight_layout()
    plt.savefig('task2_keypoints.png', dpi=300, bbox_inches='tight')
    plt.close()

    print(f"关键点可视化结果已保存至 task2_keypoints.png")

if __name__ == "__main__":
    task2()
