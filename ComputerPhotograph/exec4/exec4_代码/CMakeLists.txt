cmake_minimum_required(VERSION 3.28)
project(SolarSystemAndBouncingSpheres)
set(CMAKE_CXX_STANDARD 14)

# --- 查找 OpenGL ---
find_package(OpenGL REQUIRED)

# --- 查找 GLEW（SolarSystem 用）---
find_package(GLEW REQUIRED)
if (NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found")
endif()

# --- 查找 GLFW ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# --- 查找 GLUT（SolarSystem 需要）---
find_package(GLUT REQUIRED)

# --- 公共链接库（SolarSystem 用）---
set(COMMON_LIBS
        OpenGL::GL
        GLEW::GLEW
        ${GLFW_LIBRARIES}
        X11 Xi Xrandr Xcursor Xinerama Xxf86vm
        pthread dl
        GLUT
)

set(COMMON_INCLUDE_DIRS
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
)

# ========================================
# BouncingSpheres：手动加载 OpenGL 函数（无 GLAD / GLEW）
# ========================================

add_executable(BouncingSpheres
        bouncing_spheres.cpp
)

target_link_libraries(BouncingSpheres PRIVATE
        ${GLFW_LIBRARIES}
        GLEW::GLEW
        OpenGL::GL
        dl
        pthread
        glut          # 关键：小写 glut
        GLU
        glfw
        X11 Xi Xrandr Xcursor Xinerama Xxf86vm pthread dl
)

target_include_directories(BouncingSpheres PRIVATE
        ${GLFW_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}  # stb_image.h
)

# ========================================
# SolarSystem（完全不动）
# ========================================
add_executable(SolarSystem
        solar_system.cpp
)

# 只使用系统库，不使用可能有问题的 GLUT 目标
target_link_libraries(SolarSystem PRIVATE
        OpenGL::GL
        OpenGL::GLU
        GLEW::GLEW
        ${GLFW_LIBRARIES}
        glut          # 使用小写的库名，这是实际的库文件名
        X11 Xi Xrandr Xcursor Xinerama Xxf86vm
        pthread dl
)

target_include_directories(SolarSystem PRIVATE
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
)