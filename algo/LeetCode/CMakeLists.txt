cmake_minimum_required(VERSION 3.16)
project(LeetCodeProject)

set(CMAKE_CXX_STANDARD 20)

# 设置输出目录：所有可执行文件都放在 runtime 文件夹
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/runtime)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/runtime)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/runtime)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/runtime)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/runtime)

# 递归查找所有 cpp 文件
file(GLOB_RECURSE cpp_files CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/*.cpp)

# 遍历每个 cpp 文件
foreach(file ${cpp_files})
    get_filename_component(filename ${file} NAME_WE)

    # 读取文件内容，查找是否包含 main 函数
    file(READ ${file} contents)
    string(FIND "${contents}" "int main" main_index)

    if(NOT main_index EQUAL -1)
        # 只对含 main 的 cpp 文件创建可执行文件
        add_executable(${filename} ${file})
        set_target_properties(${filename} PROPERTIES
                OUTPUT_NAME ${filename}
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/runtime
        )
        message(STATUS "✓ Found main(): ${file} → will compile to runtime/${filename}.exe")
    else()
        message(STATUS "✗ Skipping ${file} (no main function)")
    endif()
endforeach()
