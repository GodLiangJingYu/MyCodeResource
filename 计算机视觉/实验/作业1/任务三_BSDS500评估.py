import numpy as np
import cv2
import matplotlib.pyplot as plt
from PIL import Image
import os
import scipy.io as sio
import glob

def calculate_precision_recall(detected_edges, ground_truth):
    """计算精确率和召回率"""
    # 将边缘图像转换为二值图像
    _, detected_binary = cv2.threshold(detected_edges, 127, 255, cv2.THRESH_BINARY)
    _, gt_binary = cv2.threshold(ground_truth, 127, 255, cv2.THRESH_BINARY)
    
    # 转换为浮点数并归一化
    detected_binary = detected_binary.astype(np.float32) / 255.0
    gt_binary = gt_binary.astype(np.float32) / 255.0
    
    # 计算True Positives, False Positives, False Negatives
    tp = np.sum(detected_binary * gt_binary)  # 真正例
    fp = np.sum(detected_binary * (1 - gt_binary))  # 假正例
    fn = np.sum((1 - detected_binary) * gt_binary)  # 假负例
    
    # 计算精确率和召回率
    precision = tp / (tp + fp) if (tp + fp) > 0 else 0
    recall = tp / (tp + fn) if (tp + fn) > 0 else 0
    
    return precision, recall

def task3_benchmark_evaluation():
    """任务三：使用BSDS500数据集进行边缘检测并与Ground-Truth对比"""
    print("开始任务三：BSDS500数据集边缘检测评估")
    
    # BSDS500测试集路径
    test_images_path = r"D:\MyCodeResource\计算机视觉\实验\第一次作业数据集\BSDS500\images\test"
    test_gt_path = r"D:\MyCodeResource\计算机视觉\实验\第一次作业数据集\BSDS500\groundTruth\test"
    
    if not os.path.exists(test_images_path):
        print(f"警告：找不到测试图像目录 {test_images_path}")
        return
    if not os.path.exists(test_gt_path):
        print(f"警告：找不到Ground Truth目录 {test_gt_path}")
        return
    
    # 获取测试图像列表
    image_files = glob.glob(os.path.join(test_images_path, "*.jpg"))
    if len(image_files) == 0:
        print("在测试集中未找到图像文件")
        return
    
    print(f"找到 {len(image_files)} 个测试图像")
    
    # 仅处理第一个图像以快速演示
    selected_images = image_files[:1]  # 只处理第1个图像
    
    # 用于存储结果的列表
    all_precisions = []
    all_recalls = []
    all_images_names = []
    
    for img_path in selected_images:
        img_name = os.path.basename(img_path)
        img_name_no_ext = os.path.splitext(img_name)[0]
        
        print(f"处理图像: {img_name}")
        
        # 使用PIL读取图像，然后转换为numpy数组和灰度
        try:
            pil_image = Image.open(img_path).convert('L')  # 转换为灰度图像
            image = np.array(pil_image)
            print(f"图像尺寸: {image.shape}")
        except Exception as e:
            print(f"读取图像时出错: {img_path}, 错误: {e}")
            continue
        
        # 使用Canny进行边缘检测
        best_precision = 0
        best_recall = 0
        best_edges = None
        
        # 测试不同的Canny参数
        canny_params = [(50, 100), (50, 150), (100, 200)]
        
        for low_thresh, high_thresh in canny_params:
            canny_edges = cv2.Canny(image, low_thresh, high_thresh)
            
            # 查找对应的Ground Truth文件
            gt_file = os.path.join(test_gt_path, f"{img_name_no_ext}.mat")
            if not os.path.exists(gt_file):
                print(f"未找到Ground Truth文件: {gt_file}")
                continue
            
            try:
                # 读取Ground Truth
                gt_data = sio.loadmat(gt_file)
                
                if 'groundTruth' in gt_data:
                    gt = gt_data['groundTruth']
                    print(f"Ground Truth shape: {gt.shape}")
                    
                    # 获取第一个标注
                    first_annotation = gt[0, 0]  # 1x5数组的第一个元素
                    print(f"First annotation type: {type(first_annotation)}")
                    print(f"Field names: {first_annotation.dtype.names}")
                    
                    # 获取边界图像 - 从Boundaries字段获取
                    boundaries_field = first_annotation['Boundaries']
                    print(f"Boundaries field shape: {boundaries_field.shape}")
                    
                    # boundaries_field是一个1x1数组，我们需要获取它的[0,0]元素
                    boundaries = boundaries_field[0, 0]
                    print(f"Actual boundaries shape: {boundaries.shape}, dtype: {boundaries.dtype}")
                    
                    # 调整Ground Truth图像尺寸以匹配输入图像
                    gt_resized = cv2.resize(boundaries.astype(np.float32), (image.shape[1], image.shape[0]), interpolation=cv2.INTER_NEAREST)
                    gt_resized = (gt_resized * 255).astype(np.uint8)  # 转换到[0,255]范围
                    
                    # 计算精确率和召回率
                    precision, recall = calculate_precision_recall(canny_edges, gt_resized)
                    
                    if precision > best_precision:
                        best_precision = precision
                        best_recall = recall
                        best_edges = canny_edges
                        print(f"  - 参数 ({low_thresh}, {high_thresh}): Precision={precision:.3f}, Recall={recall:.3f}")
                        
            except Exception as e:
                print(f"处理Ground Truth文件时出错 {gt_file}: {e}")
                import traceback
                traceback.print_exc()
                continue
        
        if best_edges is not None:
            all_precisions.append(best_precision)
            all_recalls.append(best_recall)
            all_images_names.append(img_name_no_ext)
            
            print(f"  - 最佳结果 {img_name_no_ext}: Precision={best_precision:.3f}, Recall={best_recall:.3f}")
        else:
            print(f"  - 无法为 {img_name_no_ext} 计算评估指标")
    
    # 显示结果
    if all_precisions and all_recalls:
        avg_precision = np.mean(all_precisions)
        avg_recall = np.mean(all_recalls)
        
        print(f"\n结果:")
        print(f"精确率: {all_precisions[0]:.3f}")
        print(f"召回率: {all_recalls[0]:.3f}")
        print(f"F1分数: {2 * all_precisions[0] * all_recalls[0] / (all_precisions[0] + all_recalls[0]) if (all_precisions[0] + all_recalls[0]) > 0 else 0:.3f}")
        
        # 显示示例图像的边缘检测结果
        if selected_images:
            img_path = selected_images[0]
            img_name = os.path.basename(img_path)
            img_name_no_ext = os.path.splitext(img_name)[0]
            
            pil_image = Image.open(img_path).convert('L')
            image = np.array(pil_image)
            
            # 使用最佳参数进行Canny边缘检测
            best_canny_edges = cv2.Canny(image, 50, 150)  # 使用示例参数
            
            # 加载Ground Truth进行显示
            gt_file = os.path.join(test_gt_path, f"{img_name_no_ext}.mat")
            gt_image = None
            if os.path.exists(gt_file):
                try:
                    gt_data = sio.loadmat(gt_file)
                    if 'groundTruth' in gt_data:
                        gt = gt_data['groundTruth']
                        first_annotation = gt[0, 0]
                        boundaries_field = first_annotation['Boundaries']
                        boundaries = boundaries_field[0, 0]
                        gt_resized = cv2.resize(boundaries.astype(np.float32), (image.shape[1], image.shape[0]), interpolation=cv2.INTER_NEAREST)
                        gt_image = (gt_resized * 255).astype(np.uint8)
                except Exception as e:
                    print(f"加载Ground Truth显示图像时出错: {e}")
            
            # 显示结果
            fig, axes = plt.subplots(1, 3, figsize=(15, 5))
            
            axes[0].imshow(image, cmap='gray')
            axes[0].set_title(f'Original: {img_name_no_ext}')
            axes[0].axis('off')
            
            axes[1].imshow(best_canny_edges, cmap='gray')
            axes[1].set_title('Canny Edge Detection')
            axes[1].axis('off')
            
            if gt_image is not None:
                axes[2].imshow(gt_image, cmap='gray')
                axes[2].set_title('Ground Truth')
                axes[2].axis('off')
            else:
                axes[2].text(0.5, 0.5, 'No GT', horizontalalignment='center', 
                             verticalalignment='center', transform=axes[2].transAxes)
                axes[2].set_title('Ground Truth')
                axes[2].axis('off')
            
            plt.tight_layout()
            plt.savefig('任务三结果/task3_sample_result.png', dpi=150, bbox_inches='tight')
            plt.show()
    
    print("任务三完成！结果已保存到任务三结果文件夹")

if __name__ == "__main__":
    task3_benchmark_evaluation()