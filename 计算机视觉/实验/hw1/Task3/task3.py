import cv2
import numpy as np
import scipy.io
import os
import glob

# Paths
# Using relative paths or absolute paths based on the workspace info
base_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
dataset_path = os.path.join(base_path, '第一次作业数据集', 'BSDS500')
img_dir = os.path.join(dataset_path, 'images', 'test')
gt_dir = os.path.join(dataset_path, 'groundTruth', 'test')
output_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'results')

if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Helper function to read image
def cv_imread(file_path, flags=cv2.IMREAD_COLOR):
    try:
        return cv2.imdecode(np.fromfile(file_path, dtype=np.uint8), flags)
    except Exception as e:
        print(f"Error reading file {file_path}: {e}")
        return None

# Get list of images
image_files = glob.glob(os.path.join(img_dir, '*.jpg'))
if not image_files:
    # Try searching recursively or check extension
    image_files = glob.glob(os.path.join(img_dir, '*.bmp')) + glob.glob(os.path.join(img_dir, '*.png'))
    if not image_files:
        print(f"No images found in {img_dir}")

print(f"Found {len(image_files)} test images.")

total_precision = 0
total_recall = 0
count = 0

# Parameters for Canny
t1, t2 = 100, 200

# Kernel for dilation (tolerance)
kernel = np.ones((3,3), np.uint8)

results_file = os.path.join(output_dir, 'evaluation_results.txt')

with open(results_file, 'w') as f:
    f.write("ImageID, Precision, Recall\n")

    for img_path in image_files:
        # File ID
        filename = os.path.basename(img_path)
        file_id = os.path.splitext(filename)[0]
        
        # Load Image
        img = cv_imread(img_path, cv2.IMREAD_GRAYSCALE)
        if img is None:
            continue
            
        # Detect Edges
        detected_edges = cv2.Canny(img, t1, t2)
        # Binarize (0 or 1)
        detected_edges_bin = (detected_edges > 0).astype(np.uint8)
        
        # Load GT
        gt_path = os.path.join(gt_dir, file_id + '.mat')
        if not os.path.exists(gt_path):
            print(f"GT not found for {file_id}")
            continue
            
        try:
            mat = scipy.io.loadmat(gt_path)
            gt_struct = mat['groundTruth']
            
            num_annotators = gt_struct.shape[1]
            
            img_p_sum = 0
            img_r_sum = 0
            
            # Prepare dilated detected edges for Recall calculation
            dilated_detected = cv2.dilate(detected_edges_bin, kernel)
            
            for i in range(num_annotators):
                # Extract boundary
                gt_boundary = gt_struct[0, i]['Boundaries'][0, 0]
                # Binarize
                gt_boundary = (gt_boundary > 0).astype(np.uint8)
                
                # Check shapes
                if gt_boundary.shape != detected_edges_bin.shape:
                    # Sometimes orientation mismatch might occur?
                    # Transpose if necessary?
                    # Usually BSDS images match correctly.
                    # But if mismatch, let's try transpose
                    if gt_boundary.shape == detected_edges_bin.T.shape:
                         gt_boundary = gt_boundary.T
                    else:
                        # Cannot compare
                        continue

                # Prepare dilated GT for Precision calculation
                dilated_gt = cv2.dilate(gt_boundary, kernel)
                
                # Precision: match detected to dilated GT
                # Numerator: detected pixels that fall onto dilated GT
                tp_prec = np.sum(detected_edges_bin & dilated_gt)
                n_detected = np.sum(detected_edges_bin)
                
                precision = tp_prec / (n_detected + 1e-8)
                
                # Recall: match GT to dilated detected
                # Numerator: GT pixels that fall onto dilated detected
                tp_rec = np.sum(gt_boundary & dilated_detected)
                n_gt = np.sum(gt_boundary)
                
                recall = tp_rec / (n_gt + 1e-8)
                
                img_p_sum += precision
                img_r_sum += recall
            
            # Average P/R for this image across annotators
            avg_p = img_p_sum / num_annotators
            avg_r = img_r_sum / num_annotators
            
            total_precision += avg_p
            total_recall += avg_r
            count += 1
            
            f.write(f"{file_id}, {avg_p:.4f}, {avg_r:.4f}\n")
            
            if count % 20 == 0:
                print(f"Processed {count} images...")
                
            # Save first image result as sample
            if count == 1:
                cv2.imencode('.png', detected_edges)[1].tofile(os.path.join(output_dir, 'sample_detected.png'))
                # Save one GT for visualization
                sample_gt = gt_struct[0, 0]['Boundaries'][0, 0] * 255
                cv2.imencode('.png', sample_gt)[1].tofile(os.path.join(output_dir, 'sample_gt.png'))

        except Exception as e:
            print(f"Error processing {file_id}: {e}")

    # Global Average
    if count > 0:
        global_avg_p = total_precision / count
        global_avg_r = total_recall / count
        print(f"Global Average Precision: {global_avg_p:.4f}")
        print(f"Global Average Recall: {global_avg_r:.4f}")
        f.write(f"AVERAGE, {global_avg_p:.4f}, {global_avg_r:.4f}\n")
    else:
        print("No images processed.")

print("Task 3 completed.")
