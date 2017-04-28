find_package( WiringPi REQUIRED )

add_executable( client client.cc )
target_link_libraries( client ${ZMQ_LIBRARIES} ${WIREPI_LIBRARIES} )

