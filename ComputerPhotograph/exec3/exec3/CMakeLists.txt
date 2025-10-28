cmake_minimum_required(VERSION 3.16)
project(TeapotLighting CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# 两个可独立运行的目标
add_executable(teapot main.cpp)
add_executable(teapot_material main2.cpp)

target_link_libraries(teapot PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
target_link_libraries(teapot_material PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)