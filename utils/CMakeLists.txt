add_library(Ucesb_launcher SHARED)
add_library(Ucesb::launcher ALIAS Ucesb_launcher)

target_sources(Ucesb_launcher PRIVATE UcesbLauncher.cpp UcesbStructInfo.cpp)
target_link_libraries(Ucesb_launcher PRIVATE Boost::thread Boost::system
                                             Ucesb::client)
target_include_directories(Ucesb_launcher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
