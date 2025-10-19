cmake_minimum_required(VERSION 3.23)
project(ComputerPhotograph)
set(CMAKE_CXX_STANDARD 14)

add_executable(ComputerPhotograph main.cpp src/glad.c)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(ComputerPhotograph ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)