#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <fstream>
#include <sys/socket.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <wiringPi.h>
//raspistill -o /home/pi/samsung/first_pic2.jpg

namespace {
	const char* take_video = "raspivid -o /home/pi/samsung/video.h264 -t 30000 -d";
	const char* remove_video = "rm /home/pi/samsung/video.h264";
	const std::string path = "/home/pi/samsung/video.h264";
	const int send_size = 163840;
}

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
	system( "gpio edge 11 falling" );
	std::cout << wiringPiSetupGpio() << std::endl;
	std::cout << wpiPinToGpio( 11 ) << std::endl;
	std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
	int send_fd = create_connection();
	std::vector<char> buffer( send_size, 0 );
	int interrupted = waitForInterrupt( 11, -1 );
	if( interrupted ) {
		std::cout << "Motion detected" << std::endl;
		system( take_video );
		setsockopt( send_fd, SOL_SOCKET, SO_SNDBUF, &send_size, sizeof( send_size ) );
		std::ifstream file( path.c_str(), std::ios_base::binary | std::ios::ate );
		file.seekg( 0, std::ios::beg );

		while( file.read( buffer.data(), buffer.size() ) ) {
			if( send( send_fd, static_cast<void *>( buffer.data() ), buffer.size(), 0 ) < 0 )
				std::cout << "Send failed" << std::endl;
		}
		file.close();
		system( remove_video );
	}
	return 0;
}

