cmake_minimum_required(VERSION 3.20)
project(exec2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加源文件
add_executable(exec2
        main.cpp
        Back4TSP_re.cpp
        BB4TSP_re.cpp
)