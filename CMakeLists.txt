cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )
project( server_client C CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" )

find_package( ZMQ REQUIRED )

if ( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64" )
      add_subdirectory( server )
endif()

if ( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv7l" )
      add_subdirectory( client )
endif()

