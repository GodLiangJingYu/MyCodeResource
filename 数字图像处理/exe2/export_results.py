"""
焊缝图像分割 - 批量导出脚本
=============================
无需GUI，直接用优化参数处理两张情景图片，导出结果。
输出风格：灰度原图 + 红色轮廓线（与参考图一致）

用法: python export_results.py
"""
import os
import sys
import cv2
import numpy as np

# 复用主程序的函数
sys.path.insert(0, os.path.dirname(__file__))
from weld_segmentation import (
    imread_cn, imwrite_cn,
    preprocess, segment, morphological_process,
    detect_edges, draw_contours_on_original,
    illumination_correction, normalize_division,
)

OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "result_export")
os.makedirs(OUTPUT_DIR, exist_ok=True)


def process_image(src_path, out_prefix, params):
    """处理单张图片并导出结果"""
    img = imread_cn(src_path, cv2.IMREAD_COLOR)
    if img is None:
        print(f"[ERROR] 无法读取: {src_path}")
        return
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    h, w = gray.shape
    print(f"\n处理: {os.path.basename(src_path)} ({w}x{h})")

    # ---- 预处理 ----
    pp = params["preprocess"]
    if pp == "clahe":
        blurred = illumination_correction(
            gray, clip_limit=params["clahe_clip"],
            tile_grid=params["clahe_grid"])
    elif pp == "clahe+blur":
        corrected = illumination_correction(
            gray, clip_limit=params["clahe_clip"],
            tile_grid=params["clahe_grid"])
        blurred = preprocess(corrected, params["blur_ksize"],
                            params["blur_type"])
    elif pp == "normalize":
        blurred = normalize_division(gray, blur_ksize=params["norm_ksize"])
    else:
        blurred = preprocess(gray, params["blur_ksize"],
                            params["blur_type"])

    # ---- 阈值分割 ----
    binary = segment(
        blurred, method=params["seg_method"],
        thresh_val=params["thresh_val"], max_val=params["thresh_max"],
        adaptive_block=params["adaptive_block"],
        adaptive_c=params["adaptive_c"])

    # ---- 形态学 ----
    morphed = morphological_process(
        binary, op_type=params["morph_op"],
        ksize=params["morph_ksize"],
        iterations=params["morph_iter"])

    # ---- 边缘检测 ----
    edges = detect_edges(
        morphed, method=params["edge_method"],
        low_thresh=params["canny_low"],
        high_thresh=params["canny_high"],
        sobel_ksize=params["edge_ksize"])

    # ---- 轮廓叠加（红色，带面积过滤）----
    min_area = params.get("min_area", 100)
    overlay = draw_contours_on_original(
        gray, morphed,
        color=(0, 0, 255),
        thickness=params.get("contour_thick", 2),
        min_area=min_area)

    # ---- 导出 ----
    paths = {
        "原图灰度": gray,
        "预处理结果": blurred,
        "阈值分割": binary,
        "形态学处理": morphed,
        "边缘检测": edges,
        "轮廓叠加(红)": overlay,
    }
    for name, img_out in paths.items():
        fname = f"{out_prefix}_{name}.png"
        fpath = os.path.join(OUTPUT_DIR, fname)
        imwrite_cn(fpath, img_out)
        print(f"  [OK] {fname}")

    weld_pixels = np.count_nonzero(morphed)
    total = morphed.shape[0] * morphed.shape[1]
    ratio = weld_pixels / total * 100
    print(f"  焊缝占比: {ratio:.1f}%")


# ======================== 各情景优化参数 ========================

SCENE1_PARAMS = {
    "preprocess": "clahe+blur",
    "blur_type": "gaussian", "blur_ksize": 5,
    "clahe_clip": 2.0, "clahe_grid": 8,
    "norm_ksize": 51,
    "seg_method": "otsu",
    "thresh_val": 127, "thresh_max": 255,
    "adaptive_block": 11, "adaptive_c": 2,
    "morph_op": "close",
    "morph_ksize": 5, "morph_iter": 1,
    "edge_method": "canny",
    "canny_low": 50, "canny_high": 150,
    "edge_ksize": 3,
    "min_area": 200,
    "contour_thick": 2,
}

SCENE2_PARAMS = {
    "preprocess": "clahe",
    "blur_type": "gaussian", "blur_ksize": 5,
    "clahe_clip": 3.0,
    "clahe_grid": 6,
    "norm_ksize": 51,
    "seg_method": "adaptive_gaussian",
    "thresh_val": 127, "thresh_max": 255,
    "adaptive_block": 25,
    "adaptive_c": 3,
    "morph_op": "open+close",
    "morph_ksize": 7, "morph_iter": 2,
    "edge_method": "canny",
    "canny_low": 30, "canny_high": 100,
    "edge_ksize": 3,
    "min_area": 300,
    "contour_thick": 2,
}


if __name__ == "__main__":
    print("=" * 50)
    print("焊缝图像分割 - 批量导出")
    print(f"输出目录: {OUTPUT_DIR}")
    print("=" * 50)

    scene1_path = os.path.join(os.path.dirname(__file__), "情景一示例1.jpg")
    if os.path.exists(scene1_path):
        process_image(scene1_path, "scene1", SCENE1_PARAMS)
    else:
        print(f"[SKIP] 找不到: {scene1_path}")

    scene2_path = os.path.join(os.path.dirname(__file__), "情景二 示例.jpg")
    if os.path.exists(scene2_path):
        process_image(scene2_path, "scene2", SCENE2_PARAMS)
    else:
        print(f"[SKIP] 找不到: {scene2_path}")

    print("\n" + "=" * 50)
    print(f"全部完成！结果已保存到: {OUTPUT_DIR}")
    print("=" * 50)
