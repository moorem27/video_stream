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
	long long int size = file.tellg();
	file.seekg( 0, std::ifstream::beg );

    long long int chunk_size = size/chunks;
	std::cout << "Chunk size: " << chunk_size << std::endl;

	int max_buffer_size = 4096;

	while( chunk_size % max_buffer_size != 0 )
		--max_buffer_size;


    std::cout << "chunk % buffer: " << max_buffer_size << std::endl;
	for( int i = chunks; i > 0; --i ) {
		auto begin = std::chrono::high_resolution_clock::now();
		++index;
		char buffer[max_buffer_size];

		long long int current_end = ( ( index * size ) / chunks );
		std::ostringstream out_file_path;
		out_file_path << "/Users/matthewmoore/Desktop/chunk" << index << ".mov";
		std::string out_file_name( out_file_path.str() );
		paths.push_back( out_file_name );
		std::ofstream out_file;
		out_file.open( out_file_name, std::ofstream::out | std::ofstream::app );
		while( file.tellg() != current_end ) {
			file.read( buffer, max_buffer_size );
			out_file << buffer;
			memset( &buffer, 0, sizeof( buffer ) );
		}
		out_file.close();
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::seconds>( end - begin ).count() << " s" << std::endl;
	}

	return paths;	
}

int main( void ) {
	const int chunks = 6;
	const std::string file_path = "/Users/matthewmoore/Desktop/silicon_valley.mov";
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

// TODO: In case things go horribly, horribly wrong...(like they are)
//void react_to_motion( const int send_fd ) {
//    std::vector<char> buffer( send_size, 0 );
//    system( take_video );
//    std::cout << "Finished taking video" << std::endl;
//    std::ifstream file( video_path.c_str(), std::ios_base::binary | std::ios::ate );
//    file.seekg( 0, std::ios::beg );
//
//    while( file.read( buffer.data(), buffer.size() ) ) {
//        if( send( send_fd, static_cast<void *>( buffer.data() ), buffer.size(), 0 ) < 0 )
//            std::cout << "Send failed" << std::endl;
//    }
//    std::cout << "Finished sending video" << std::endl;
//    file.close();
//    system( remove_video );
//}
//
//
//
//int main( void ) {
//    std::cout << wiringPiSetupGpio() << std::endl;
//    const int send_fd = create_connection();
//    std::cout << "send_fd " << send_fd << std::endl;
//    std::this_thread::sleep_for( std::chrono::seconds( 15 ) );
//
//    while( true ) {
//        if( digitalRead( 7 ) ) {
//            std::cout << "Motion detected!" << std::endl;
//            react_to_motion( send_fd );
//            break;
//        }
//    }
//    return 0;
//}



