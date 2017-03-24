#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <fstream>
#include <sys/socket.h>
#include <iostream>

int create_connection() {
	struct sockaddr_in server_connection{};
	int file_descriptor = socket( PF_INET, SOCK_STREAM, 0 );

	memset( &server_connection, 0, sizeof( server_connection ) );
	server_connection.sin_family = AF_INET;
	server_connection.sin_addr.s_addr = inet_addr( "192.168.1.12" );
	server_connection.sin_port = htons( 8888 );

	int timeout = 0;
	while( ++timeout < 100 ) {
		connect( file_descriptor, reinterpret_cast<struct sockaddr*>( &server_connection ), sizeof( server_connection ) );
		}
	return file_descriptor;
} 	
		
int main( void ) {
	int n = 0;
	unsigned int m = sizeof( n );
	std::string path = "/home/pi/samsung/arma.mp4";
	int send_fd = create_connection();
	int send_buffer_size = 106496;
	int result = setsockopt( send_fd, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, sizeof( send_buffer_size ) );
	getsockopt(send_fd,  SOL_SOCKET, SO_RCVBUF, (void*)&n, &m );
	std::cout << "Buffer size: " << n << std::endl;

	std::ifstream file( path.c_str(), std::ios_base::binary | std::ios::ate );
	std::streamsize size = file.tellg();
	std::cout << "std::streamsize: " << size << std::endl;
	file.seekg( 0, std::ios::beg );
	std::vector<char> buffer( size );
	std::cout << "Made the vector" << std::endl;
	if( file.read( buffer.data(), size ) )
		std::cout << "Amount of data sent: " << send( send_fd, static_cast<void *>( buffer.data() ), size, 0 ) << std::endl;

	return 0;
}

