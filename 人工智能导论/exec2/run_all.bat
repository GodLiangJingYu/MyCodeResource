@echo off
echo Running Task 1: Basic Loss Recording...
call conda run -n cnn_env python task1_basic_loss/main.py
echo.
echo ----------------------------------------------------------------
echo.
echo Running Task 2: Depth Impact (Shallow - 1 Layer)...
call conda run -n cnn_env python task2_depth_impact/cnn_shallow.py
echo.
echo Running Task 2: Depth Impact (Standard - 2 Layers)...
call conda run -n cnn_env python task2_depth_impact/cnn_depth_2.py
echo.
echo Running Task 2: Depth Impact (Deep - 3 Layers)...
call conda run -n cnn_env python task2_depth_impact/cnn_deep.py
echo.
echo Running Task 2: Depth Impact (Deeper - 4 Layers)...
call conda run -n cnn_env python task2_depth_impact/cnn_depth_4.py
echo.
echo Running Task 2: Depth Impact (Deepest - 5 Layers)...
call conda run -n cnn_env python task2_depth_impact/cnn_depth_5.py
echo.
echo ----------------------------------------------------------------
echo.
echo Running Task 3: Kernel Impact (Kernel 3x3)...
call conda run -n cnn_env python task3_kernel_impact/cnn_kernel_3.py
echo.
echo Running Task 3: Kernel Impact (Kernel 5x5)...
call conda run -n cnn_env python task3_kernel_impact/cnn_kernel_5.py
echo.
echo Running Task 3: Kernel Impact (Kernel 7x7)...
call conda run -n cnn_env python task3_kernel_impact/cnn_kernel_7.py
echo.
echo Running Task 3: Kernel Impact (Kernel 9x9)...
call conda run -n cnn_env python task3_kernel_impact/cnn_kernel_9.py
echo.
echo All tasks completed.
pause
