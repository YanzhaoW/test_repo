cmake_minimum_required(VERSION 3.24)

project(test_ucesb)

set(CMAKE_CXX_STANDARD 23)

if(DEFINED BOOST_DIR)
  list(APPEND CMAKE_PREFIX_PATH ${BOOST_DIR})
else()
  message(FATAL_ERROR "Please set BOOST_DIR")
endif()
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Ucesb REQUIRED MODULE)
find_package(Boost REQUIRED COMPONENTS system thread)

add_subdirectory(utils)

add_executable(test main.cpp)
target_link_libraries(test PRIVATE Ucesb::land Ucesb::client Ucesb::launcher
                                   Boost::thread)
