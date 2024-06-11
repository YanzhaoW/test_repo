cmake_minimum_required(VERSION 3.27)

project(test_root)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_PREFIX_PATH $ENV{SIMPATH})
list(APPEND CMAKE_PREFIX_PATH $ENV{FAIRROOTPATH})

find_package(FairCMakeModules 0.2 REQUIRED)
include(FairFindPackage2)
find_package2(PRIVATE FairRoot CONFIG VERSION 19 REQUIRED)
list(PREPEND CMAKE_MODULE_PATH ${FairRoot_PREFIX}/share/fairbase/cmake/modules)

find_package2_implicit_dependencies()

include(FairSummary)
fair_summary_package_dependencies()

add_subdirectory(data)

add_executable(write_data write.cpp)
target_link_libraries(write_data PRIVATE FairRoot::Base myData)

add_executable(read_data read.cpp)
target_link_libraries(read_data PRIVATE FairRoot::Base myData)
