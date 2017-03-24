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
	std::string path = "/home/pi/samsung/kayla_video.mov";
	int send_fd = create_connection();
	std::ifstream file( path.c_str(), std::ios_base::binary | std::ios::ate );
	std::streamsize size = file.tellg();
	file.seekg( 0, std::ios::beg );
	std::vector<char> buffer( size );
	std::cout << "Made the vector" << std::endl;
	if( file.read( buffer.data(), size ) )
		std::cout << "Amount of data sent: " << send( send_fd, static_cast<void *>( buffer.data() ), size, 0 ) << std::endl;

	return 0;
}

