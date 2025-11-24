cmake_minimum_required(VERSION 3.10)
project(OpenGLTest)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenGL
find_package(OpenGL REQUIRED)

# 查找GLEW
find_package(GLEW REQUIRED)

# 查找GLFW
find_package(glfw3 REQUIRED)

# 包含目录
# GLM是纯头文件库，我们在这里添加它的路径
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(vendor) # 假设GLM会放在vendor文件夹下

# 创建可执行文件
add_executable(OpenGLTest main.cpp)

# 创建GDI绘图演示可执行文件
add_executable(GraphicsDemo graphics_demo.cpp)

# 将着色器文件复制到可执行文件的目录
file(COPY
    vertex.shader
    fragment.shader
    DESTINATION ${CMAKE_BINARY_DIR}/bin
)

# 链接库 (GLM是纯头文件库, 不需要链接)
target_link_libraries(OpenGLTest 
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
)

# 链接GDI库用于GraphicsDemo
target_link_libraries(GraphicsDemo gdi32 user32)

# Windows特定设置
if(WIN32)
    # 添加必要的库
    target_link_libraries(OpenGLTest opengl32)
endif()

# 设置输出目录
set_target_properties(OpenGLTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(GraphicsDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)