cmake_minimum_required(VERSION 3.8)
project(undistort)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED core imgproc highgui calib3d)

add_executable(undistort
               undistort_main.cpp)
target_link_libraries(undistort PRIVATE ${OpenCV_LIBS})
target_include_directories(undistort PRIVATE SYSTEM ${OpenCV_INCLUDE_DIRS})
