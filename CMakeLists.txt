cmake_minimum_required(VERSION 3.27)

project(test_root)

find_package(ROOT REQUIRED CONFIG)

add_subdirectory(data)

add_executable(write_data write.cpp)
target_link_libraries(write_data PRIVATE ROOT::RIO ROOT::Tree myData)

add_executable(read_data read.cpp)
target_link_libraries(read_data PRIVATE ROOT::RIO ROOT::Tree myData)
