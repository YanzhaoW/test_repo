message(STATUS "Trying to import ucesb client library ...")

if(DEFINED UCESB_DIR)
  add_library(Ucesb_clnt SHARED IMPORTED)
  set_target_properties(
    Ucesb_clnt PROPERTIES IMPORTED_LOCATION
                          ${UCESB_DIR}/hbook/libext_data_clnt.so)
  target_include_directories(Ucesb_clnt INTERFACE ${UCESB_DIR}/hbook
                                                  ${CMAKE_SOURCE_DIR}/utils)

  add_library(Ucesb_land INTERFACE)
  target_include_directories(Ucesb_land INTERFACE ${UCESB_DIR}/land)
  target_compile_definitions(Ucesb_land
                             INTERFACE LAND_EXECUTABLE="${UCESB_DIR}/land/land")

  add_library(Ucesb::client ALIAS Ucesb_clnt)
  add_library(Ucesb::land ALIAS Ucesb_land)
  message(STATUS "Ucesb has been imported successfully!")
else()
  message(FATAL_ERROR "Please define UCESB_DIR")
endif()
