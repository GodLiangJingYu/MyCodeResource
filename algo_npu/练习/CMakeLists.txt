cmake_minimum_required(VERSION 3.16)
project(EXERCISE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录设置
set(RUNTIME_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/runtime)
foreach(OUTPUTCONFIG DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RUNTIME_OUTPUT_DIR})
endforeach()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIR})

# 查找所有 cpp 文件
file(GLOB_RECURSE ALL_CPP CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/*.cpp)

# 自动为含 main 的 .cpp 创建可执行文件
foreach(CPP_FILE ${ALL_CPP})
    file(READ "${CPP_FILE}" CONTENTS)
    string(FIND "${CONTENTS}" "int main" HAS_MAIN)
    if(NOT HAS_MAIN EQUAL -1)
        get_filename_component(EXE_NAME "${CPP_FILE}" NAME_WE)
        add_executable(${EXE_NAME} ${CPP_FILE})
        set_target_properties(${EXE_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIR}
                OUTPUT_NAME ${EXE_NAME}
        )
        message(STATUS "Detected main(): ${CPP_FILE}, will build runtime/${EXE_NAME}.exe")
    else()
        message(STATUS "Skip: ${CPP_FILE} (no main function)")
    endif()
endforeach()