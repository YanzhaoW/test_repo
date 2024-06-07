message(STATUS "Trying to import ucesb client library ...")

if(DEFINED UCESB_DIR)
  add_library(Ucesb_clnt SHARED IMPORTED)
  set_target_properties(
    Ucesb_clnt PROPERTIES IMPORTED_LOCATION
                          ${UCESB_DIR}/hbook/libext_data_clnt.so)
  target_include_directories(Ucesb_clnt INTERFACE ${UCESB_DIR}/hbook
                                                  ${CMAKE_SOURCE_DIR}/utils)

  add_library(Ucesb::client ALIAS Ucesb_clnt)
  message(STATUS "Ucesb has been imported successfully!")
else()
  message(FATAL_ERROR "Please define UCESB_DIR")
endif()
