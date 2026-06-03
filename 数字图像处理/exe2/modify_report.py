# -*- coding: utf-8 -*-
#!/usr/bin/env python3
"""脚本：自动修改实验报告，添加代码片段、完整代码和结果图像"""
import os
import re

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPORT_PATH = os.path.join(SCRIPT_DIR, "实验报告.md")
SOURCE_PATH = os.path.join(SCRIPT_DIR, "weld_segmentation.py")

# 读取源文件
with open(SOURCE_PATH, "r", encoding="utf-8") as f:
    source_code = f.read()

# 读取报告
with open(REPORT_PATH, "r", encoding="utf-8") as f:
    report = f.read()

# ============================================================
# 修改1：在每个原理小节下面添加对应的代码片段
# ============================================================

# --- 3.1 图像预处理代码片段 ---
preprocess_code = """

**对应代码实现：**

```python
def preprocess(image, blur_ksize=5, blur_type="gaussian"):
    """预处理：去噪平滑。支持高斯/中值/双边三种滤波方式"""
    k = blur_ksize if blur_ksize % 2 == 1 else blur_ksize + 1  # 确保核大小为奇数
    if blur_type == "gaussian":
        return cv2.GaussianBlur(image, (k, k), 0)
    elif blur_type == "median":
        return cv2.medianBlur(image, k)
    elif blur_type == "bilateral":
        return cv2.bilateralFilter(image, k, 75, 75)  # 空间/灰度域标准差
    return image
```"""

# --- 3.2 阈值分割代码片段 ---
segment_code = """

**对应代码实现：**

```python
def segment(image, method="otsu", thresh_val=127, max_val=255,
            adaptive_block=11, adaptive_c=2):
    """焊缝区域分割：支持 Otsu/手动二值化/自适应阈值 五种方法"""
    if method == "otsu":
        _, binary = cv2.threshold(image, 0, max_val,
                                  cv2.THRESH_BINARY + cv2.THRESH_OTSU)
        return binary
    elif method == "triangle":
        _, binary = cv2.threshold(image, 0, max_val,
                                  cv2.THRESH_BINARY + cv2.THRESH_TRIANGLE)
        return binary
    elif method == "manual" or method == "binary":
        _, binary = cv2.threshold(image, thresh_val, max_val,
                                  cv2.THRESH_BINARY)
        return binary
    elif method == "binary_inv":
        _, binary = cv2.threshold(image, thresh_val, max_val,
                                  cv2.THRESH_BINARY_INV)
        return binary
    elif method == "adaptive_mean":
        b = adaptive_block if adaptive_block % 2 == 1 else adaptive_block + 1
        return cv2.adaptiveThreshold(image, max_val,
                                     cv2.ADAPTIVE_THRESH_MEAN_C,
                                     cv2.THRESH_BINARY, b, adaptive_c)
    elif method == "adaptive_gaussian":
        b = adaptive_block if adaptive_block % 2 == 1 else adaptive_block + 1
        return cv2.adaptiveThreshold(image, max_val,
                                     cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                     cv2.THRESH_BINARY, b, adaptive_c)
    return image
```"""

# --- 3.3 形态学处理代码片段 ---
morph_code = """

**对应代码实现：**

```python
def morphological_process(binary, op_type="close", ksize=5, iterations=1):
    """形态学处理：开/闭/膨胀/腐蚀，用于去噪和填补空洞"""
    k = ksize if ksize % 2 == 1 else ksize + 1
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (k, k))  # 矩形结构元素
    ops = {
        "open": cv2.MORPH_OPEN,    # 先腐蚀后膨胀：去噪
        "close": cv2.MORPH_CLOSE,  # 先膨胀后腐蚀：填空洞
        "dilate": cv2.MORPH_DILATE,
        "erode": cv2.MORPH_ERODE,
    }
    return cv2.morphologyEx(binary, ops.get(op_type, cv2.MORPH_CLOSE),
                            kernel, iterations=iterations)
```"""

# --- 3.4 边缘检测代码片段 ---
edge_code = """

**对应代码实现：**

```python
def detect_edges(image, method="canny", low_thresh=50, high_thresh=150,
                 sobel_ksize=3):
    """边缘检测：Canny/Sobel/Laplacian/Scharr 四种方法"""
    if method == "canny":
        return cv2.Canny(image, low_thresh, high_thresh)
    elif method == "sobel":
        k = sobel_ksize if sobel_ksize % 2 == 1 else sobel_ksize + 1
        k = max(1, min(k, 7))
        gx = cv2.Sobel(image, cv2.CV_64F, 1, 0, ksize=k)  # x 方向梯度
        gy = cv2.Sobel(image, cv2.CV_64F, 0, 1, ksize=k)  # y 方向梯度
        mag = np.sqrt(gx ** 2 + gy ** 2)                    # 梯度幅值
        if mag.max() > 0:                                    # 防止除零
            mag = mag / mag.max() * 255                      # 归一化到 [0,255]
        return np.uint8(np.clip(mag, 0, 255))
    elif method == "laplacian":
        k = sobel_ksize if sobel_ksize % 2 == 1 else sobel_ksize + 1
        k = max(1, min(k, 7))
        lap = cv2.Laplacian(image, cv2.CV_64F, ksize=k)     # 二阶导数
        return np.uint8(np.clip(np.abs(lap), 0, 255))
    elif method == "scharr":
        gx = cv2.Scharr(image, cv2.CV_64F, 1, 0)
        gy = cv2.Scharr(image, cv2.CV_64F, 0, 1)
        mag = np.sqrt(gx ** 2 + gy ** 2)
        if mag.max() > 0:
            mag = mag / mag.max() * 255
        return np.uint8(np.clip(mag, 0, 255))
    return image
```"""

# --- 3.5 灰度直方图代码片段 ---
hist_code = """

**对应代码实现（GUI 中的直方图绘制与阈值标注）：**

```python
# 绘制灰度直方图
self.ax_hist.hist(blurred.ravel(), bins=256, range=(0, 256),
                  color="steelblue", alpha=0.8, linewidth=0.5)
self.ax_hist.set_title("灰度直方图", fontsize=10)

# 手动二值化模式下标注阈值线
if seg_method in ("manual", "binary", "binary_inv"):
    t = self._get_val("thresh_val")
    self.ax_hist.axvline(x=t, color="red", linestyle="--",
                        linewidth=1.2, label=f"阈值={t}")

# Otsu 模式下计算并标注最优阈值
elif seg_method == "otsu":
    hist = cv2.calcHist([blurred], [0], None, [256], [0, 256])
    total = blurred.shape[0] * blurred.shape[1]
    # 遍历所有阈值，计算类间方差，取最大值对应的阈值
    sum_total = np.dot(np.arange(256), hist.ravel())
    sum_bg, w_bg = 0.0, 0.0
    max_var, best_t = 0, 0
    for t_i in range(256):
        w_bg += hist[t_i][0]
        if w_bg == 0: continue
        w_fg = total - w_bg
        if w_fg == 0: break
        sum_bg += t_i * hist[t_i][0]
        mean_bg = sum_bg / w_bg
        mean_fg = (sum_total - sum_bg) / w_fg
        var = w_bg * w_fg * (mean_bg - mean_fg) ** 2
        if var > max_var:
            max_var = var
            best_t = t_i
    self.ax_hist.axvline(x=best_t, color="red", linestyle="--",
                        linewidth=1.2, label=f"Otsu阈值≈{best_t}")
```"""

# 执行替换：在每个小节的描述文本后插入代码
# 3.1: 在双边滤波描述后、3.2前插入
report = report.replace(
    "- **双边滤波（Bilateral Filter）**：同时考虑空间距离和像素值差异，在平滑噪声的同时保留强边缘，适合焊缝这种需要保持清晰边界的场景。\n\n### 3.2",
    "- **双边滤波（Bilateral Filter）**：同时考虑空间距离和像素值差异，在平滑噪声的同时保留强边缘，适合焊缝这种需要保持清晰边界的场景。" + preprocess_code + "\n\n### 3.2"
)

# 3.2: 在自适应阈值描述后、3.3前插入
report = report.replace(
    "- **自适应阈值（Adaptive Mean / Adaptive Gaussian）**：对每个像素计算其局部邻域的均值或高斯加权均值作为阈值，适合光照不均匀的图像。\n\n### 3.3",
    "- **自适应阈值（Adaptive Mean / Adaptive Gaussian）**：对每个像素计算其局部邻域的均值或高斯加权均值作为阈值，适合光照不均匀的图像。" + segment_code + "\n\n### 3.3"
)

# 3.3: 在腐蚀描述后、3.4前插入
report = report.replace(
    "- **腐蚀（Erode）**：收缩白色区域，能去除细小连接。\n\n### 3.4",
    "- **腐蚀（Erode）**：收缩白色区域，能去除细小连接。" + morph_code + "\n\n### 3.4"
)

# 3.4: 在Scharr算子描述后、3.5前插入
report = report.replace(
    "- **Scharr 算子**：Sobel 算子的优化版本，核系数更精确，对梯度的近似更准确。\n\n### 3.5",
    "- **Scharr 算子**：Sobel 算子的优化版本，核系数更精确，对梯度的近似更准确。" + edge_code + "\n\n### 3.5"
)

# 3.5: 在描述文本后、##四前插入
report = report.replace(
    '绘制预处理后图像的灰度直方图（256 个灰度级），并标注当前阈值线。通过观察直方图的峰谷分布，可以直观判断最优阈值位置，辅助用户调整参数。\n\n## 四',
    '绘制预处理后图像的灰度直方图（256 个灰度级），并标注当前阈值线。通过观察直方图的峰谷分布，可以直观判断最优阈值位置，辅助用户调整参数。' + hist_code + '\n\n## 四'
)

# ============================================================
# 修改2：替换第五节为完整代码
# ============================================================

source_line_count = source_code.count('\n') + 1
section5_new = f"""## 五、程序代码及注释

完整代码见 `weld_segmentation.py`（共 {source_line_count} 行）。以下为完整源代码：

```python
{source_code.strip()}
```
"""

# 替换第五节内容（从"## 五"到"## 六"之前）
section5_pattern = r'## 五、程序代码及注释.*?(?=## 六、实验结果)'
report = re.sub(section5_pattern, section5_new.rstrip(), report, flags=re.DOTALL)

# ============================================================
# 修改3：在第六节添加结果图像
# ============================================================

scene1_images = """
**结果图像：**

| 二值分割结果 | 形态学处理结果 |
|:---:|:---:|
| ![二值分割](result/情景一示例1_binary.png) | ![形态学处理](result/情景一示例1_morphed.png) |

| 边缘检测结果 | 轮廓叠加结果 | 对比图 |
|:---:|:---:|:---:|
| ![边缘检测](result/情景一示例1_edges.png) | ![轮廓叠加](result/情景一示例1_overlay.png) | ![对比图](result/情景一示例1_comparison.png) |
"""

scene2_images = """
**结果图像：**

| 二值分割结果 | 形态学处理结果 |
|:---:|:---:|
| ![二值分割](result/情景二%20示例_binary.png) | ![形态学处理](result/情景二%20示例_morphed.png) |

| 边缘检测结果 | 轮廓叠加结果 | 对比图 |
|:---:|:---:|:---:|
| ![边缘检测](result/情景二%20示例_edges.png) | ![轮廓叠加](result/情景二%20示例_overlay.png) | ![对比图](result/情景二%20示例_comparison.png) |
"""

report = report.replace(
    "（结果图像保存于目录中，文件名格式：`result_scene1_*.png`）",
    scene1_images.strip()
)

report = report.replace(
    "（结果图像保存于目录中，文件名格式：`result_scene2_*.png`）",
    scene2_images.strip()
)

# ============================================================
# 写回文件
# ============================================================
with open(REPORT_PATH, "w", encoding="utf-8") as f:
    f.write(report)

print(f"✅ 报告已修改完成！")
print(f"   - 第三节：每个原理小节下已添加对应代码片段")
print(f"   - 第五节：已替换为完整源代码（{source_line_count} 行）")
print(f"   - 第六节：已添加场景一和场景二的结果图像")
print(f"   - 文件：{REPORT_PATH}")
