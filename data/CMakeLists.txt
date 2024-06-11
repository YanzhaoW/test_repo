# add_library(myData SHARED MyData.cpp)
# target_include_directories(myData PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(myData PRIVATE ROOT::RIO)

add_library(myData INTERFACE)
target_include_directories(myData INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY(myData_dict MyData.hpp MODULE myData LINKDEF DataLinkDef.h)
