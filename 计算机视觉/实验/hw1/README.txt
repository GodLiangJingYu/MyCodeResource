第一次作业完成情况总结

Task 1: 基本处理
- 代码文件: Task1/task1.py
- 功能: 对lena灰度图和彩色图进行高斯滤波和LoG边缘检测。
- 参数: 窗口大小 3x3 到 11x11 (步长2), Sigma 1 到 7 (步长2)。
- 结果: 保存在 Task1/results 文件夹下。
- 注意: 为了处理中文路径，使用了 cv2.imdecode 和 cv2.imencode。

Task 2: Canny检测子
- 代码文件: Task2/task2.py
- 功能: 对lena灰度图进行Canny边缘检测。
- 参数: 测试了多组阈值 (50, 150), (100, 200), (30, 100) 等。
- 结果: 保存在 Task2/results 文件夹下。

Task 3: BSDS500数据集评估
- 代码文件: Task3/task3.py
- 功能: 对BSDS500测试集进行Canny边缘检测，并与Ground Truth进行对比计算精确率(Precision)和召回率(Recall)。
- 方法: 使用Canny (阈值100, 200)，允许1像素误差(使用3x3膨胀)。
- 结果:
  - Global Average Precision: 0.1564
  - Global Average Recall: 0.7113
  - 详细结果保存在 Task3/results/evaluation_results.txt
  - 样本结果保存在 Task3/results/sample_detected.png 和 sample_gt.png

运行说明:
在各个Task目录下运行对应的python脚本即可。
例如: python Task1/task1.py

