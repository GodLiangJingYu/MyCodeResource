import numpy as np
import cv2
import matplotlib.pyplot as plt

def imread_chinese(path):
    """读取中文路径的图片"""
    import os
    img = cv2.imdecode(np.frombuffer(open(os.path.abspath(path), 'rb').read(), np.uint8), -1)
    return img

def sobel_filter(image):
    """使用Sobel算子计算图像梯度"""
    # Sobel算子
    sobel_x = np.array([[-1, 0, 1],
                        [-2, 0, 2],
                        [-1, 0, 1]])

    sobel_y = np.array([[-1, -2, -1],
                        [0, 0, 0],
                        [1, 2, 1]])

    # 获取图像尺寸
    height, width = image.shape

    # 初始化梯度图像
    ix = np.zeros((height, width))
    iy = np.zeros((height, width))

    # 卷积操作
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            patch = image[i-1:i+2, j-1:j+2]
            ix[i, j] = np.sum(patch * sobel_x)
            iy[i, j] = np.sum(patch * sobel_y)

    return ix, iy

def harris_corner_detector(image, window_size=3, alpha=0.05, threshold=0.01):
    """Harris角点检测算法"""
    # 转换为灰度图
    if len(image.shape) == 3:
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    else:
        gray = image.copy()

    # 转换为浮点型
    gray = gray.astype(np.float32) / 255.0

    # 计算梯度
    ix, iy = sobel_filter(gray)

    # 计算M矩阵的元素
    ixx = ix * ix
    iyy = iy * iy
    ixy = ix * iy

    # 高斯窗口权重
    offset = window_size // 2
    kernel = cv2.getGaussianKernel(window_size, 1.5)
    kernel = kernel * kernel.T

    # 计算角点响应函数
    height, width = gray.shape
    response = np.zeros((height, width))

    for i in range(offset, height - offset):
        for j in range(offset, width - offset):
            # 提取窗口
            ixx_window = ixx[i-offset:i+offset+1, j-offset:j+offset+1]
            iyy_window = iyy[i-offset:i+offset+1, j-offset:j+offset+1]
            ixy_window = ixy[i-offset:i+offset+1, j-offset:j+offset+1]

            # 加权求和
            sxx = np.sum(ixx_window * kernel)
            syy = np.sum(iyy_window * kernel)
            sxy = np.sum(ixy_window * kernel)

            # 计算M矩阵
            M = np.array([[sxx, sxy], [sxy, syy]])

            # 计算角点响应函数 R = det(M) - alpha * trace(M)^2
            det_M = np.linalg.det(M)
            trace_M = np.trace(M)
            R = det_M - alpha * (trace_M ** 2)

            response[i, j] = R

    # 阈值处理
    max_response = np.max(response)
    threshold_value = max_response * threshold

    # 提取角点
    corners = []
    for i in range(offset, height - offset):
        for j in range(offset, width - offset):
            if response[i, j] > threshold_value:
                # 非极大值抑制
                is_max = True
                for di in [-1, 0, 1]:
                    for dj in [-1, 0, 1]:
                        if di == 0 and dj == 0:
                            continue
                        if response[i+di, j+dj] > response[i, j]:
                            is_max = False
                            break
                    if not is_max:
                        break
                if is_max:
                    corners.append((j, i))

    return response, corners

def visualize_corners(image, corners, title="Harris角点检测"):
    """可视化角点检测结果"""
    result = image.copy()
    if len(result.shape) == 2:
        result = cv2.cvtColor(result, cv2.COLOR_GRAY2BGR)

    for corner in corners:
        x, y = corner
        cv2.circle(result, (int(x), int(y)), 3, (0, 0, 255), -1)

    return result

def task1():
    """任务一：Harris角点检测"""
    # 读取图像
    image_path = "check_board.png"
    image = imread_chinese(image_path)
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # 设置不同参数进行对比
    params = [
        {"window_size": 3, "alpha": 0.05, "threshold": 0.01},
        {"window_size": 5, "alpha": 0.05, "threshold": 0.01},
        {"window_size": 3, "alpha": 0.05, "threshold": 0.02},
        {"window_size": 5, "alpha": 0.05, "threshold": 0.005},
    ]

    fig, axes = plt.subplots(2, 3, figsize=(18, 12))
    fig.suptitle('Harris角点检测不同参数对比', fontsize=16, fontproperties='SimHei')

    # 显示原始图像
    axes[0, 0].imshow(image_rgb)
    axes[0, 0].set_title('原始图像', fontproperties='SimHei')
    axes[0, 0].axis('off')

    idx = 1
    for param in params:
        response, corners = harris_corner_detector(image,
                                                  window_size=param["window_size"],
                                                  alpha=param["alpha"],
                                                  threshold=param["threshold"])

        result = visualize_corners(image, corners)

        row = idx // 3
        col = idx % 3
        axes[row, col].imshow(cv2.cvtColor(result, cv2.COLOR_BGR2RGB))
        axes[row, col].set_title(f'窗口大小={param["window_size"]}, 阈值={param["threshold"]}\n角点数量={len(corners)}',
                                fontproperties='SimHei')
        axes[row, col].axis('off')

        idx += 1

    # 隐藏最后一个未使用的子图
    axes[1, 2].axis('off')

    plt.tight_layout()
    plt.savefig('task1_result.png', dpi=300, bbox_inches='tight')
    plt.close()

    print(f"任务一完成！")
    print(f"参数1: 窗口大小=3, alpha=0.05, 阈值=0.01, 角点数量={len(harris_corner_detector(image, 3, 0.05, 0.01)[1])}")
    print(f"参数2: 窗口大小=5, alpha=0.05, 阈值=0.01, 角点数量={len(harris_corner_detector(image, 5, 0.05, 0.01)[1])}")
    print(f"参数3: 窗口大小=3, alpha=0.05, 阈值=0.02, 角点数量={len(harris_corner_detector(image, 3, 0.05, 0.02)[1])}")
    print(f"参数4: 窗口大小=5, alpha=0.05, 阈值=0.005, 角点数量={len(harris_corner_detector(image, 5, 0.05, 0.005)[1])}")
    print(f"结果已保存至 task1_result.png")

if __name__ == "__main__":
    task1()
