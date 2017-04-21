cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )
project( server_client CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_EXTENSIONS OFF )

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" )
find_package( ZMQ REQUIRED )
find_package( WiringPi REQUIRED )
find_package( Threads )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lwiringPi")


add_executable( server server.cc )
target_link_libraries( server ${ZMQ_LIBRARIES} )

add_executable( client client.cc )
target_link_libraries( client
   ${CMAKE_THREAD_LIBS_INIT}
   ${ZMQ_LIBRARIES}
   ${WIREPI_LIBRARIES} )
