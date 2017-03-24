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
	server_connection.sin_addr.s_addr = inet_addr( "192.168.1.3" );
	server_connection.sin_port = htons( 8888 );

	int timeout = 0;
	while( ++timeout < 100 ) {
		connect( file_descriptor, reinterpret_cast<struct sockaddr*>( &server_connection ), sizeof( server_connection ) );
		}
	return file_descriptor;
} 	
		
int main( void ) {
	// 163840  (/proc/sys/net/core/rmem_max)
	std::string path = "/home/pi/samsung/sv.mp4";
	int send_fd = create_connection();
	ssize_t send_size = 163840; 
	std::ifstream file( path.c_str(), std::ios_base::binary | std::ios::ate );
	std::cout << "Made file" << std::endl;
	file.seekg( 0, std::ios::beg );
	std::vector<char> buffer( send_size,0 );
	std::cout << "Made the vector" << std::endl;
	
	while( file.read( buffer.data(), buffer.size() ) ) {
		send( send_fd, static_cast<void *>( buffer.data() ), buffer.size(), 0 );
	}	
	return 0;
}

