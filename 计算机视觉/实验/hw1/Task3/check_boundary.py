import scipy.io
import os
import numpy as np

mat_path = r'd:\MyCodeResource\计算机视觉\实验\hw1\第一次作业数据集\BSDS500\groundTruth\test\100007.mat'
mat = scipy.io.loadmat(mat_path)
gt = mat['groundTruth']
# Check first annotator
annot0 = gt[0, 0]
boundaries = annot0['Boundaries'][0, 0]
print("Boundaries shape:", boundaries.shape)
print("Boundaries dtype:", boundaries.dtype)
print("Unique values:", np.unique(boundaries))
