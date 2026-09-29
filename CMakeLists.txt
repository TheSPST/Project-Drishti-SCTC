cmake_minimum_required(VERSION 3.10)
project(SCTC_Live_Webcam)

set(CMAKE_CXX_STANDARD 20)

# Find OpenCV installed via Homebrew
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(sctc_live sctc_live_webcam.cpp)
target_link_libraries(sctc_live ${OpenCV_LIBS})
