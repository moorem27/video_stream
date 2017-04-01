cmake_minimum_required( VERSION 3.5 )
project( server_client )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

#add_executable( server server.cc )
add_executable( client client.cc )
