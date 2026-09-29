cmake_minimum_required(VERSION 3.10)
project(Project_Drishti_SCTC)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(drishti_node src/main.cpp src/SCTC_Codec.cpp)
target_link_libraries(drishti_node ${OpenCV_LIBS})
