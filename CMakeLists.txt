cmake_minimum_required(VERSION 3.10)
project(Project_Drishti_SCTC)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

# App 1: The Sender Node (Mac A)
add_executable(drishti_sender src/sender_node.cpp src/SCTC_Codec.cpp src/UDP_Transport.cpp)
target_link_libraries(drishti_sender ${OpenCV_LIBS})

# App 2: The Receiver Node (Mac B)
add_executable(drishti_receiver src/receiver_node.cpp src/SCTC_Codec.cpp src/UDP_Transport.cpp)
target_link_libraries(drishti_receiver ${OpenCV_LIBS})
