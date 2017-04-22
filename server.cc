#include <chrono>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <zmq.h>
#include <zmq.hpp>
#include "zmq_addon.hpp"

// TODO Replace local file paths with vagrant/docker path
int main( void ) {
	zmq::context_t context{ 1 };
	zmq::socket_t socket{ context, ZMQ_PULL };

        // System command to convert file format	
	const char* convert =  "ffmpeg -i /Users/matthewmoore/Desktop/first_vid.h264 -c copy /Users/matthewmoore/Desktop/first_vid.mp4";

        // Buffer size	
	size_t buffer_size = 8192;

	// Receive buffer
	char byte[ buffer_size ];
	
	// Clear
	memset( &byte, 0, sizeof( byte ) );

	std::cout << "Waiting for connection..." << std::endl;
        
	// Bind server
	socket.bind( "tcp://*:5555" );	
	std::cout << "Connected!" << std::endl;
	std::ofstream output_file{};
	output_file.open( "/Users/matthewmoore/Desktop/first_vid.h264", std::ios_base::binary | std::ios::out );

	double total_bytes = 0;
	double received_bytes = 0;
	zmq::message_t received{ buffer_size };
	auto begin = std::chrono::high_resolution_clock::now();
	do {
		if( socket.recv( &received ) ) {
		    memcpy( byte, static_cast<char*>( received.data() ), sizeof( received ) );
		    received_bytes = received.size();
		    total_bytes = total_bytes + received_bytes;
		    std::cout << '\r'<< "Received: " << total_bytes/1000000000 << std::flush;
		    output_file.write( byte, received.size() );
		} else {
			std::cout << "Closing file!" << std::endl;
			output_file.close();
			break;
		}
	} while( true );
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>( end - begin ).count();
	system( convert );
	system( "rm /Users/matthewmoore/Desktop/first_vid.h264" );
	std::cout << "\nReceived file in " << duration << " s" << std::endl;
	return 0;
}
