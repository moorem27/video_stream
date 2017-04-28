cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )
project( client C CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" )

find_package( ZMQ REQUIRED )
find_package( WiringPi REQUIRED )

add_executable( client client.cc )
target_link_libraries( client ${ZMQ_LIBRARIES} ${WIREPI_LIBRARIES} )

