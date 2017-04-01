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
#include <sstream>
//#include <wiringPi.h>

namespace {
	const char* take_picture = "raspistill -o /home/pi/samsung/motion_pic.jpg";
	const char* take_video = "raspivid -o /home/pi/samsung/video.h264 -t 30000 -d";
	const char* remove_video = "rm /home/pi/samsung/video.h264";
	const char* remove_pic = "rm /home/pi/samsung/motion_pic.jpg";
	const std::string video_path = "/home/pi/samsung/video.h264";
	const std::string pic_path = "/home/pi/samsung/motion_pic.jpg";
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
	
	setsockopt( file_descriptor, SOL_SOCKET, SO_SNDBUF, &send_size, sizeof( send_size ) );
	return file_descriptor;
} 	



void send_chunk( const std::string file_path ) {
	const int send_fd = create_connection();
	std::vector<char> buffer( send_size, 0 );
	std::cout << "Finished taking video" << std::endl;
	std::ifstream file( file_path, std::ios_base::binary | std::ios::ate );
	file.seekg( 0, std::ios::beg );

	while( file.read( buffer.data(), buffer.size() ) ) {
		if( send( send_fd, static_cast<void *>( buffer.data() ), buffer.size(), 0 ) < 0 )
			std::cout << "Send failed" << std::endl;
		std::fill( buffer.begin(), buffer.end(), 0 );
	}
	std::cout << "Finished sending video" << std::endl;
	file.close();
	system( remove_video );
}


std::vector<std::string> chunk_file( const int chunks, const std::string file_path ) {
	int index = 0;
	std::ifstream file;
	std::vector<std::string> paths{};
	file.open( file_path, std::ifstream::in );
	
	file.seekg( 0, std::ifstream::end );
	int size = file.tellg();
	file.seekg( 0, std::ifstream::beg );
	
	for( int i = chunks; i > 0; --i ) {
		++index;
		int current_end = ( ( index * size ) / chunks );
		std::ostringstream file_path;
		file_path << "/home/matt/Desktop/file" << index << ".txt";
		std::string file_name( file_path.str() );
		paths.push_back( file_name );	
		std::ofstream out_file;
		out_file.open( file_name, std::ofstream::out | std::ofstream::app );
		while( file.tellg() != current_end ) {
			out_file << static_cast<char>( file.get() );
		}
		out_file.close();
	}

	return paths;	
}

int main( void ) {
	const int chunks = 6;
	const std::string file_path = "/home/matt/q1.txt";
	std::vector<std::string> paths = chunk_file( chunks, file_path );
	std::vector<std::thread> threads{};
	for( const auto& path : paths ) {
		std::cout << path << std::endl;
	}
	// Seek to beginning of file
	// From beginning of file to ( index * size ) / chunks, write chars to file
	// When you reach temp end, open a new file, and from where you already are start writing characters to new file
	// repeat every ( index * size ) / chunks

//	std::cout << wiringPiSetupGpio() << std::endl;
//	const int send_fd = create_connection();
//	std::cout << "send_fd " << send_fd << std::endl;
//	std::this_thread::sleep_for( std::chrono::seconds( 15 ) );
//
//	while( true ) {
//		// TODO Use signal interrupts
//		if( digitalRead( 7 ) ) {
//			system( take_video );
//			std::vector<std::string> paths = chunk_file( 6, video_path );
			for( const auto& path : paths ) {
				
			}
//			std::cout << "Motion detected!" << std::endl;
//			break;
//		}
//		std::this_thread::sleep_for( std::chrono::milliseconds( 1500 ) );
  //      }
	return 0;
}



