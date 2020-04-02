cmake_minimum_required(VERSION 3.15)
project(opencv)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)

add_executable(opencv src/main.cpp src/boost_tree.cpp src/decision_tree.cpp src/utilities/file.h src/utilities/file.cpp src/utilities/constants.h src/forest.cpp src/utilities/Runner.cpp src/utilities/Runner.h src/utilities/Logger.cpp src/utilities/Logger.h)
target_link_libraries(opencv ${OpenCV_LIBS})