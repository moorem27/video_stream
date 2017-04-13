cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )
project( server_client CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_EXTENSIONS OFF )

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" )
find_package( ZMQ REQUIRED )

include_directories( include/ )

add_executable( server server.cc )
target_link_libraries( server ${ZMQ_LIBRARIES} )

add_executable( client client.cc )
target_link_libraries( client ${ZMQ_LIBRARIES} )
