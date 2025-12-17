# Artificial Intelligence Experiment 2 - CNN on MNIST

This project implements the requirements for Experiment 2 using PyTorch.

## Structure

*   **task1_basic_loss/**: Contains `main.py` which trains a standard CNN and records the loss history to `loss_history.txt`.
*   **task2_depth_impact/**: Comparison of different network depths.
    *   `cnn_shallow.py` (1 conv layer)
    *   `cnn_depth_2.py` (2 conv layers - Standard)
    *   `cnn_deep.py` (3 conv layers)
    *   `cnn_depth_4.py` (4 conv layers)
    *   `cnn_depth_5.py` (5 conv layers - VGG style)
*   **task3_kernel_impact/**: Comparison of different kernel sizes.
    *   `cnn_kernel_3.py` (3x3 kernels)
    *   `cnn_kernel_5.py` (5x5 kernels - Standard)
    *   `cnn_kernel_7.py` (7x7 kernels)
    *   `cnn_kernel_9.py` (9x9 kernels)

## How to Run

Ensure you have Anaconda installed and the `cnn_env` environment created with PyTorch.

To run all experiments sequentially, execute the `run_all.bat` script:

```bat
run_all.bat
```

Or run individual scripts manually:

```bat
conda run -n cnn_env python task1_basic_loss/main.py
conda run -n cnn_env python task2_depth_impact/cnn_shallow.py
# ... etc
```
