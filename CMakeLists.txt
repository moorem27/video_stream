cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )
project( server_client CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_EXTENSIONS OFF )

add_executable( server server.cc )
add_executable( client client.cc )
