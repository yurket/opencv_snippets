cmake_minimum_required(VERSION 2.8)

project(DisplayImage)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(tut tutorial.cc)

target_link_libraries(tut ${OpenCV_LIBS})
