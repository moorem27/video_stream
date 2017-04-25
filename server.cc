#include <chrono>
#include <string.h>
#include <fstream>
#include <iostream>
#include <zmq.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

// TODO Replace local file paths with vagrant/docker path
// TODO Break apart this hideous main function

int main( void ) {
	zmq::context_t context{ 1 };
	zmq::socket_t socket{ context, ZMQ_PULL };

	// System command to convert file format
	const char* convert =  "ffmpeg -i /Users/matthewmoore/Desktop/first_vid.h264 -c copy /Users/matthewmoore/Desktop/first_vid.mp4";

	// Buffer size
	size_t buffer_size = 10000;

	// Receive buffer
	char byte[ buffer_size ];
	
	// Clear
	memset( &byte, 0, sizeof( byte ) );

	std::cout << "Waiting for connection..." << std::endl;
        
	// Connect socket
	socket.connect( "tcp://192.168.1.5:5555" );
	std::cout << "Connected!" << std::endl;
	std::ofstream output_file;
	output_file.open( "/Users/matthewmoore/Desktop/first_vid.h264", std::ios_base::binary | std::ios::out );

	long total_bytes = 0;
	long file_size = 0;
	size_t received_bytes = 0;
	zmq::multipart_t received {};
	bool got_size = false;
	auto begin = std::chrono::high_resolution_clock::now();
	do {
		if( received.recv( socket ) ) {
			try {
				file_size = received.poptyp<int>();
				if( !got_size ) {
					std::cout << "file_size = " << file_size << std::endl;
					got_size = true;
				}

			} catch( std::runtime_error& e ) {
				std::cout << e.what() << std::endl;
			}

			const auto encoded_message = received.pop();

		    memcpy( byte, encoded_message.data<char>(), encoded_message.size() );
		    received_bytes = encoded_message.size();
		    total_bytes += received_bytes;
		    output_file.write( byte, received_bytes );
			std::cout << '\r' << total_bytes << std::flush;

			if( total_bytes == file_size ) {
				std::cout << "total_bytes == file_size" << std::endl;
				system( convert );
				system( "rm /Users/matthewmoore/Desktop/first_vid.h264" );
				file_size = 0;
				total_bytes = 0;
				received_bytes = 0;
			}
		} else {
			std::cout << "Closing file!" << std::endl;
			output_file.close();
			break;
		}
	} while( true );
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>( end - begin ).count();
	std::cout << "\nReceived file in " << duration << " s" << std::endl;
	return 0;
}
