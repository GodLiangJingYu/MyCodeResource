import cv2
import numpy as np
import os

# Define paths
# Using relative paths or absolute paths based on the workspace info
base_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
dataset_path = os.path.join(base_path, '第一次作业数据集', 'lena')
lena_gray_path = os.path.join(dataset_path, 'lena512.bmp')
output_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'results')

if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def cv2_imread(file_path, flags=cv2.IMREAD_COLOR):
    """Helper function to read images with non-ASCII paths on Windows."""
    try:
        return cv2.imdecode(np.fromfile(file_path, dtype=np.uint8), flags)
    except Exception as e:
        print(f"Error reading file {file_path}: {e}")
        return None

def cv2_imwrite(file_path, img):
    """Helper function to write images with non-ASCII paths on Windows."""
    try:
        cv2.imencode(os.path.splitext(file_path)[1], img)[1].tofile(file_path)
        return True
    except Exception as e:
        print(f"Error writing file {file_path}: {e}")
        return False

def process_canny():
    img = cv2_imread(lena_gray_path, cv2.IMREAD_GRAYSCALE)
    if img is None:
        print(f"Error: Could not load image at {lena_gray_path}")
        return

    # Define parameter sets (threshold1, threshold2)
    params = [
        (50, 150),   # Standard
        (100, 200),  # Higher thresholds
        (30, 100),   # Lower thresholds
        (100, 120),  # Narrow range
        (50, 250)    # Wide range
    ]

    for t1, t2 in params:
        edges = cv2.Canny(img, t1, t2)
        filename = f"lena_canny_t1_{t1}_t2_{t2}.png"
        save_path = os.path.join(output_dir, filename)
        cv2_imwrite(save_path, edges)
        print(f"Processed Canny: Thresholds ({t1}, {t2}) -> {filename}")

if __name__ == "__main__":
    print("Starting Task 2: Canny Edge Detection...")
    process_canny()
    print("Task 2 Complete. Results saved in 'results' folder.")
