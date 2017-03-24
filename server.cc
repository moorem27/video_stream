#include <chrono>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>

int create_server() {
	int val = 1;
	int backlog = 5;
	struct sockaddr_in server_address{};
	int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
	setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof( val ) );
	memset( &server_address, 0, sizeof( server_address ) );

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	server_address.sin_port = htons( 8888 );

	bind( listenfd, reinterpret_cast<sockaddr*>( &server_address ), sizeof( server_address ) );

	listen( listenfd, backlog );
	return listenfd;
}

int main( void ) {
	
	int listen_fd = create_server();
	int buffer_size = 212992;
	char byte[ buffer_size ];
	memset( &byte, 0, sizeof( byte ) );
	std::cout << "Waiting for connection..." << std::endl;	
	int ack_fd = accept( listen_fd, 0, 0 );
	std::cout << "Connected!" << std::endl;
	setsockopt( ack_fd, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof( buffer_size ) );
	std::ofstream output_file{};
	output_file.open( "/home/matt/Desktop/received.mov", std::ios_base::binary | std::ios::out );

	int bytes = buffer_size;
	double total_bytes = 0;
	double received_bytes = 0;
	auto begin = std::chrono::high_resolution_clock::now();
	do {

		received_bytes = recv( ack_fd, byte, buffer_size, MSG_WAITALL );
		if( received_bytes > 0 ) {
			output_file.write( byte, buffer_size );
			std::cout << '\r'<< "Received: " << total_bytes/1000000000 << " GB" << std::flush;
			total_bytes = total_bytes + received_bytes;
			memset( &byte, 0, sizeof( byte ) );
		} else {
			output_file.close();
			break;
		}
	} while( true );
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>( end - begin ).count();
	std::cout << "\nReceived file in " << duration << " s" << std::endl;
	return 0;
}
