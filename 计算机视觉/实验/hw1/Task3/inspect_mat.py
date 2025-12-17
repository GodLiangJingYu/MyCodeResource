import scipy.io
import os

mat_path = r'd:\MyCodeResource\计算机视觉\实验\hw1\第一次作业数据集\BSDS500\groundTruth\test\100007.mat'

if not os.path.exists(mat_path):
    print(f"File not found: {mat_path}")
else:
    try:
        mat = scipy.io.loadmat(mat_path)
        print("Keys in mat file:", mat.keys())
        
        # Inspect 'groundTruth' key which is likely the one
        if 'groundTruth' in mat:
            gt = mat['groundTruth']
            print("Shape of groundTruth:", gt.shape)
            print("Type of groundTruth:", type(gt))
            
            # Usually it's a cell array of structs
            # Let's inspect the first element
            print("Number of annotators:", gt.shape[1])
            for i in range(gt.shape[1]):
                annot = gt[0, i]
                print(f"Annotator {i} data structure:", annot.dtype)
                # It seems each annotation might have 'Segmentation' and 'Boundaries'
                # We need 'Boundaries' for edge detection evaluation
                
    except Exception as e:
        print("Error loading mat file:", e)
