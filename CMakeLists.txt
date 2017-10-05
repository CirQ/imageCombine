cmake_minimum_required(VERSION 3.8)
project(imageCombine)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.1.0 REQUIRED)
include_directories("/usr/include/opencv2")

set(SOURCE_FILES main.cpp)
add_executable(imageCombine ${SOURCE_FILES})
target_link_libraries(imageCombine opencv_core)
target_link_libraries(imageCombine opencv_imgcodecs)
target_link_libraries(imageCombine opencv_highgui)
