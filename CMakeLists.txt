cmake_minimum_required(VERSION 3.28)

project(project_name)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(asio REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE asio::asio)
target_compile_options(main PRIVATE -fno-omit-frame-pointer -fsanitize=memory)
target_link_options(main PUBLIC -fsanitize=memory)
