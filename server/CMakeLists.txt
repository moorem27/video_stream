add_executable( server server.cc )
target_link_libraries( server ${ZMQ_LIBRARIES} )
