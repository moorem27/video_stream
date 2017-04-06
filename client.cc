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


// TODO Move all helper functions into a library file
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


// TODO Take in a port number to create a unique connection
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


// TODO Use vector for dynamically sizeable buffer
std::vector<std::string> chunk_file( const int chunks, const std::string file_path ) {
	std::ifstream file;
	std::vector<std::string> paths{};

	// Open binary file
	file.open( file_path, std::ios_base::binary );

	// Seek to end
	file.seekg( 0, std::ifstream::end );

	// Grab character position
	long long int size = file.tellg();
	std::cout << "Total file size: " << size << std::endl;
	// Seek back to beginning;
	file.seekg( 0, std::ifstream::beg );

	// Chunk size = total size / num chunks
    long long int chunk_size = size/chunks;
	std::cout << "Chunk size: " << chunk_size << std::endl;

	// Start max buffer size at arbitrarily large number
	int max_buffer_size = 10000;

	// Reach a buffer size the will divide evenly into the chunk size
	while( chunk_size % max_buffer_size != 0 )
		max_buffer_size--;

	std::cout << "max_buffer_size: " << max_buffer_size << std::endl;

	for( int i = 1; i <= chunks; ++i ) {
		std::cout << "i = " << i << std::endl;
		char buffer[ max_buffer_size ];
		memset( &buffer, 0, sizeof( buffer ) );

		long long int current_end = ( ( i * size ) / chunks );
		std::cout << "current_end: " << current_end << std::endl;
		std::ostringstream out_file_path;
		out_file_path << "/home/matt/Desktop/chunk" << i << ".mp4";
		std::string out_file_name( out_file_path.str() );
		paths.push_back( out_file_name );
		std::ofstream out_file;
		out_file.open( out_file_name, std::ios_base::binary | std::ios::out );
		long long int last = 0;

		if( out_file.is_open() ) {
			out_file.seekp(0, std::ios_base::beg);
			while ( static_cast<long long int>( file.tellg() ) <= current_end && file.read( buffer, max_buffer_size ) ) {
				out_file.write( buffer, sizeof( buffer ) );
				memset( &buffer, 0, sizeof( buffer ) );
				last = file.tellg();
			}

			if ( size % last == 1 ) {
				file.read( buffer, sizeof( char ) );
				out_file.write( buffer, sizeof( char ) );
			}

		}
		std::cout << "file.tellg() " << last << std::endl;
		out_file.close();

	}

	return paths;	
}

void combine_files( const std::vector<std::string>& paths, const std::string out_path ) {
	std::ofstream output( out_path, std::ios_base::binary | std::ios::out );

	for( const auto& path : paths ) {
		std::ifstream file( path, std::ios_base::binary );
		output << file.rdbuf();
	}
}

int test_chunks( void ) {
	const int chunks = 3;
	const std::string file_path = "/home/matt/Desktop/arma.mp4";
	auto begin = std::chrono::high_resolution_clock::now();
	std::vector<std::string> paths = chunk_file( chunks, file_path );
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>( end - begin ).count() << " s" << std::endl;

	combine_files( paths, "/home/matt/Desktop/example.mp4" );


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
int main( void ) {
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
//        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
//    }
	test_chunks();
    return 0;
}



