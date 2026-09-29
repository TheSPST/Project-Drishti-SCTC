cmake_minimum_required(VERSION 3.10)
project(Project_Drishti_SCTC)

set(CMAKE_CXX_STANDARD 20)

# [KALKI AGI] Detect Apple Silicon (M1/M2/M3) and append the Homebrew prefix 
# so CMake can automatically find OpenCV without manual path flags.
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(drishti_node src/main.cpp src/SCTC_Codec.cpp)
target_link_libraries(drishti_node ${OpenCV_LIBS})
