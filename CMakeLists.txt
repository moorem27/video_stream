cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )
project( server_client C CXX )

message( STATUS "Architecture -> ${CMAKE_SYSTEM_PROCESSOR}")

if ( (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86") OR (${CMAKE_SYSTEM_PROCESSOR} MATCHES "amd") )
      add_subdirectory( server )
elseif ( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm" )
      add_subdirectory( client )
else()
      message( FATAL_ERROR "Architecture not supported -> ${CMAKE_SYSTEM_PROCESSOR}")
endif()


