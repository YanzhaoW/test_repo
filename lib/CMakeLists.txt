add_library(mylib SHARED MyClass.cxx)

target_link_libraries(mylib SHARED ROOT::Base Boost::headers)

root_generate_dictionary(
    mylib_dict
    MyClass.hpp
    MODULE
    mylib
    LINKDEF
    MyLibLinkDef.h)
