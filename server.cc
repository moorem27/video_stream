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
	char byte[ 1024 ];
	memset( &byte, 0, sizeof( byte ) );
	
	int ack_fd = accept( listen_fd, 0, 0 );

	std::ofstream output_file{};
	output_file.open( "/home/matt/Desktop/received.mov", std::ios_base::binary | std::ios::out );

	do {
		if( recv( ack_fd, byte, 512, MSG_WAITALL ) > 0 ) {
			output_file.write( byte, 512 );
			memset( &byte, 0, sizeof( byte ) );
		} else {
			output_file.close();
			break;
		}
	} while( true );
	std::cout << "Received file" << std::endl;
	return 0;
}
