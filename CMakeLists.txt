cmake_minimum_required(VERSION 3.26)


project(root_asio)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(ROOT REQUIRED)
find_package(Boost REQUIRED COMPONENTS headers)

add_subdirectory(lib)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE mylib)
