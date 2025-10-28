cmake_minimum_required(VERSION 3.10)
project(OpenGLProjects)

set(CMAKE_CXX_STANDARD 11)

# 设置 OpenGL 偏好为 LEGACY，避免 GLVND 警告
set(OpenGL_GL_PREFERENCE LEGACY)

# 查找 OpenGL 和 GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# 调试输出，确保库路径正确
message("OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")
message("GLUT_LIBRARY: ${GLUT_LIBRARY}")

# 如果 GLUT_LIBRARY 为空，显式指定
if(NOT GLUT_LIBRARY)
    set(GLUT_LIBRARY /usr/lib/x86_64-linux-gnu/libglut.so)
endif()

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# 程序 1
add_executable(Program1 main1.cpp)
target_link_libraries(Program1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} X11 Xi Xmu)

# 程序 2
add_executable(Program2 main2.cpp)
target_link_libraries(Program2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} X11 Xi Xmu)

# 程序 3
add_executable(Program3 main3.cpp)
target_link_libraries(Program3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} X11 Xi Xmu)

# 程序 4
add_executable(Program4 main4.cpp)
target_link_libraries(Program4 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} X11 Xi Xmu)