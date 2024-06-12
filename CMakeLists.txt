cmake_minimum_required(VERSION 3.27)

project(test_root)

# set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_PREFIX_PATH $ENV{SIMPATH})
list(APPEND CMAKE_PREFIX_PATH $ENV{FAIRROOTPATH})

find_package(FairCMakeModules 0.2 REQUIRED)
include(FairFindPackage2)
find_package2(PRIVATE FairRoot CONFIG REQUIRED)
list(PREPEND CMAKE_MODULE_PATH ${FairRoot_PREFIX}/share/fairbase/cmake/modules)

if(FairRoot_VERSION VERSION_LESS "19")
  find_package2(PRIVATE ROOT CONFIG REQUIRED)
  find_package2(PRIVATE FairLogger CONFIG REQUIRED)
  find_package2(PRIVATE VMC CONFIG REQUIRED)
endif()

find_package2_implicit_dependencies()

include(FairSummary)
fair_summary_package_dependencies()

add_subdirectory(data)

add_executable(write_data write_task.cpp)
target_link_libraries(write_data PRIVATE FairRoot::Base myData)

add_executable(read_data read_task.cpp)
target_link_libraries(read_data PRIVATE FairRoot::Base myData)

add_executable(read_data_raw read.cpp)
target_link_libraries(read_data_raw PRIVATE myData ROOT::ROOTDataFrame)

if(FairRoot_VERSION VERSION_LESS "19")
  target_link_directories(read_data PRIVATE ${ROOT_LIBRARY_DIR})
  target_link_directories(write_data PRIVATE ${ROOT_LIBRARY_DIR})
  target_compile_definitions(read_data PRIVATE FAIRROOT18=1)
  target_compile_definitions(write_data PRIVATE FAIRROOT18=1)
  target_link_libraries(write_data PRIVATE ROOT::RIO)
  target_link_libraries(read_data PRIVATE ROOT::RIO)
endif()
