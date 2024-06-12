add_library(myData SHARED)
target_include_directories(myData PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(myData PRIVATE ROOT::RIO)

root_generate_dictionary(myData_dict MyData.hpp MODULE myData LINKDEF
                         DataLinkDef.h)
