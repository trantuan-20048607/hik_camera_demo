cmake_minimum_required(VERSION 3.21)
project(hik_camera_demo)

set(MVS_PATH /opt/MVS)

# Optimizer
set(CMAKE_CXX_COMPILER /usr/bin/g++-9)
add_compile_options(-O3 -std=c++2a -flto -march=native)

# Options for EasyLogging++
add_compile_options(
        -DELPP_THREAD_SAFE
        -DELPP_UNICODE
        -DELPP_STACKTRACE_ON_CRASH
        -DELPP_NO_DEFAULT_LOG_FILE
)

find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${MVS_PATH}/include)
file(GLOB MVS_LIBS ${MVS_PATH}/lib/64/*.so)

include_directories(/usr/include/x86_64-linux-gnu)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

file(GLOB LOGGING_SRC ${CMAKE_CURRENT_SOURCE_DIR}/log/*.c*)

add_executable(hik_camera_demo ${SRC} ${LOGGING_SRC})

target_link_libraries(hik_camera_demo
        ${MVS_LIBS}
        ${OpenCV_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        )
