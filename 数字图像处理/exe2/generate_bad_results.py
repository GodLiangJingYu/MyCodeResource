#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""批量生成不佳参数组合的处理结果图片"""

import os
import sys
import cv2
import numpy as np
from weld_segmentation import (
    imread_cn, imwrite_cn, preprocess, segment,
    morphological_process, detect_edges, draw_contours_on_original
)

# 输出目录
OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "result", "bad_params")
os.makedirs(OUT_DIR, exist_ok=True)

# 图像路径
IMG1 = os.path.join(os.path.dirname(os.path.abspath(__file__)), "情景一示例1.jpg")
IMG2 = os.path.join(os.path.dirname(os.path.abspath(__file__)), "情景二 示例.jpg")


def process_and_save(image_path, name, params):
    """处理并保存结果"""
    img = imread_cn(image_path)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # 预处理
    pp = params.get("preprocess", {})
    pp_type = pp.get("type", "blur_only")
    if pp_type == "none":
        blurred = gray.copy()
    elif pp_type == "blur_only":
        blurred = preprocess(gray, pp.get("ksize", 5), pp.get("blur_type", "gaussian"))
    elif pp_type == "median":
        blurred = preprocess(gray, pp.get("ksize", 5), "median")
    elif pp_type == "bilateral":
        blurred = preprocess(gray, pp.get("ksize", 5), "bilateral")
    else:
        blurred = preprocess(gray, pp.get("ksize", 5), pp.get("blur_type", "gaussian"))

    # 分割
    seg = params.get("segment", {})
    binary = segment(blurred, method=seg.get("method", "otsu"),
                     thresh_val=seg.get("thresh_val", 127),
                     max_val=255,
                     adaptive_block=seg.get("adaptive_block", 11),
                     adaptive_c=seg.get("adaptive_c", 2))

    # 形态学
    morph = params.get("morphological", {})
    if morph.get("skip", False):
        morphed = binary.copy()
    else:
        morphed = morphological_process(binary, op_type=morph.get("op", "close"),
                                        ksize=morph.get("ksize", 5),
                                        iterations=morph.get("iter", 1))

    # 边缘
    edge = params.get("edge", {})
    edges = detect_edges(morphed, method=edge.get("method", "canny"),
                         low_thresh=edge.get("low", 50),
                         high_thresh=edge.get("high", 150),
                         sobel_ksize=edge.get("ksize", 3))

    # 轮廓叠加
    overlay = draw_contours_on_original(img, morphed, thickness=2)

    base = os.path.join(OUT_DIR, name)
    imwrite_cn(f"{base}_binary.png", binary)
    imwrite_cn(f"{base}_morphed.png", morphed)
    imwrite_cn(f"{base}_edges.png", edges)
    imwrite_cn(f"{base}_overlay.png", overlay)

    # 拼接对比图（原图灰度 | 分割 | 边缘 | 叠加）
    h, w = gray.shape
    comparison = np.zeros((h, w * 4, 3), dtype=np.uint8)
    comparison[:, :w] = cv2.cvtColor(gray, cv2.COLOR_GRAY2BGR)
    comparison[:, w:2*w] = cv2.cvtColor(binary, cv2.COLOR_GRAY2BGR)
    comparison[:, 2*w:3*w] = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)
    comparison[:, 3*w:] = overlay
    imwrite_cn(f"{base}_comparison.png", comparison)

    print(f"✓ 已保存: {name}")


# ===================== 情景一不佳参数 =====================

# 1. 无预处理 + Otsu
process_and_save(IMG1, "情景一_01_无预处理_Otsu", {
    "preprocess": {"type": "none"},
    "segment": {"method": "otsu"},
    "morphological": {"op": "close", "ksize": 5, "iter": 1},
    "edge": {"method": "canny", "low": 50, "high": 150}
})

# 2. 手动阈值过低 (80)
process_and_save(IMG1, "情景一_02_阈值过低_80", {
    "preprocess": {"type": "blur_only", "ksize": 5, "blur_type": "gaussian"},
    "segment": {"method": "binary", "thresh_val": 80},
    "morphological": {"op": "close", "ksize": 5, "iter": 1},
    "edge": {"method": "canny", "low": 50, "high": 150}
})

# 3. 跳过形态学处理
process_and_save(IMG1, "情景一_03_无形态学", {
    "preprocess": {"type": "blur_only", "ksize": 5, "blur_type": "gaussian"},
    "segment": {"method": "otsu"},
    "morphological": {"skip": True},
    "edge": {"method": "canny", "low": 50, "high": 150}
})

# 4. Canny 低阈值过低 (20)
process_and_save(IMG1, "情景一_04_Canny低阈值20", {
    "preprocess": {"type": "blur_only", "ksize": 5, "blur_type": "gaussian"},
    "segment": {"method": "otsu"},
    "morphological": {"op": "close", "ksize": 5, "iter": 1},
    "edge": {"method": "canny", "low": 20, "high": 150}
})

# 5. 中值滤波核过大 (15)
process_and_save(IMG1, "情景一_05_中值核15", {
    "preprocess": {"type": "median", "ksize": 15},
    "segment": {"method": "otsu"},
    "morphological": {"op": "close", "ksize": 5, "iter": 1},
    "edge": {"method": "canny", "low": 50, "high": 150}
})

# ===================== 情景二不佳参数 =====================

# 1. 全局 Otsu
process_and_save(IMG2, "情景二_01_全局Otsu", {
    "preprocess": {"type": "blur_only", "ksize": 5, "blur_type": "gaussian"},
    "segment": {"method": "otsu"},
    "morphological": {"op": "close", "ksize": 5, "iter": 1},
    "edge": {"method": "canny", "low": 50, "high": 150}
})

# 2. 自适应块过小 (7)
process_and_save(IMG2, "情景二_02_自适应块7", {
    "preprocess": {"type": "blur_only", "ksize": 5, "blur_type": "gaussian"},
    "segment": {"method": "adaptive_gaussian", "adaptive_block": 7, "adaptive_c": 2},
    "morphological": {"op": "close", "ksize": 5, "iter": 1},
    "edge": {"method": "canny", "low": 50, "high": 150}
})

# 3. 形态学核过大 (21)
process_and_save(IMG2, "情景二_03_形态学核21", {
    "preprocess": {"type": "bilateral", "ksize": 9},
    "segment": {"method": "adaptive_gaussian", "adaptive_block": 21, "adaptive_c": 5},
    "morphological": {"op": "close", "ksize": 21, "iter": 2},
    "edge": {"method": "canny", "low": 30, "high": 100}
})

# 4. 双边滤波核过小 (3)
process_and_save(IMG2, "情景二_04_双边核3", {
    "preprocess": {"type": "bilateral", "ksize": 3},
    "segment": {"method": "adaptive_gaussian", "adaptive_block": 21, "adaptive_c": 5},
    "morphological": {"op": "close", "ksize": 7, "iter": 1},
    "edge": {"method": "canny", "low": 30, "high": 100}
})

# 5. 自适应常数 C 过大 (15)
process_and_save(IMG2, "情景二_05_自适应C15", {
    "preprocess": {"type": "bilateral", "ksize": 9},
    "segment": {"method": "adaptive_gaussian", "adaptive_block": 21, "adaptive_c": 15},
    "morphological": {"op": "close", "ksize": 7, "iter": 1},
    "edge": {"method": "canny", "low": 30, "high": 100}
})

print("\n✓ 所有不佳参数结果已生成！")
print(f"输出目录: {OUT_DIR}")
