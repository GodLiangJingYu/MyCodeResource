import cv2
import numpy as np
import matplotlib.pyplot as plt
import os

# Define paths
lena_gray_path = r'd:\MyCodeResource\计算机视觉\实验\hw1\第一次作业数据集\lena\lena512.bmp'
lena_color_path = r'd:\MyCodeResource\计算机视觉\实验\hw1\第一次作业数据集\lena\lena512color.tiff'
output_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'results')

# Create output directory if it doesn't exist
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Load images
def cv_imread(file_path, flags=cv2.IMREAD_COLOR):
    return cv2.imdecode(np.fromfile(file_path, dtype=np.uint8), flags)

img_gray = cv_imread(lena_gray_path, cv2.IMREAD_GRAYSCALE)
img_color = cv_imread(lena_color_path) # BGR format
if img_color is not None:
    img_color_rgb = cv2.cvtColor(img_color, cv2.COLOR_BGR2RGB) # RGB for plotting

if img_gray is None:
    print(f"Error: Could not load gray image from {lena_gray_path}")
if img_color is None:
    print(f"Error: Could not load color image from {lena_color_path}")

# Parameters
window_sizes = list(range(3, 12, 2)) # 3, 5, 7, 9, 11
scales = list(range(1, 8, 2)) # 1, 3, 5, 7

def apply_gaussian_and_log(image, ksize, sigma, is_gray=True):
    # Gaussian Blur
    blurred = cv2.GaussianBlur(image, (ksize, ksize), sigma)

    # Laplacian of Gaussian (LoG)
    # Apply Laplacian to the Gaussian blurred image
    # cv2.Laplacian depth needs to be CV_64F to avoid overflow/underflow, then take absolute value
    if is_gray:
        laplacian = cv2.Laplacian(blurred, cv2.CV_64F, ksize=ksize)
        edge_strength = np.uint8(np.absolute(laplacian))
    else:
        # For color images, apply to each channel or convert to gray?
        # Requirement says "detect edges" (calculate edge strength).
        # Common approach for color edge detection is handling channels separately or combining them.
        # Let's apply to each channel and take the maximum or L2 norm.
        # Here we will apply Laplacian to each channel and combine.
        laplacian = cv2.Laplacian(blurred, cv2.CV_64F, ksize=ksize)
        # laplacian will be multi-channel
        # Calculate magnitude
        edge_strength = np.max(np.absolute(laplacian), axis=2).astype(np.uint8)

    return blurred, edge_strength

# cv2_imwrite function
def cv2_imwrite(file_path, img):
    """Helper function to write images with non-ASCII paths on Windows."""
    try:
        cv2.imencode(os.path.splitext(file_path)[1], img)[1].tofile(file_path)
        return True
    except Exception as e:
        print(f"Error writing file {file_path}: {e}")
        return False

# Iterate and process
for ksize in window_sizes:
    for sigma in scales:
        print(f"Processing: Window Size={ksize}, Sigma={sigma}")

        # Gray Image
        blurred_gray, edge_gray = apply_gaussian_and_log(img_gray, ksize, sigma, is_gray=True)

        # Color Image
        blurred_color, edge_color = apply_gaussian_and_log(img_color, ksize, sigma, is_gray=False)

        # Save results
        suffix = f"k{ksize}_s{sigma}"

        # Save Gray results
        cv2_imwrite(os.path.join(output_dir, f"lena_gray_blur_{suffix}.png"), blurred_gray)
        cv2_imwrite(os.path.join(output_dir, f"lena_gray_edge_{suffix}.png"), edge_gray)

        # Save Color results
        cv2_imwrite(os.path.join(output_dir, f"lena_color_blur_{suffix}.png"), blurred_color)
        cv2_imwrite(os.path.join(output_dir, f"lena_color_edge_{suffix}.png"), edge_color)

print("Task 1 completed. Results saved to", output_dir)
