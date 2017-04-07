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
	const int max_buffer_size = 20000;
	char buffer[ max_buffer_size ];
}

std::string get_extension( const std::string file_path ) {
	size_t last_index = file_path.find_last_of( "." );
	std::string extension = file_path.substr( last_index, file_path.length() );
	return extension;
}

std::string remove_extension( const std::string file_path ) {
	size_t dot = file_path.find_last_of( "." );
	if ( dot == std::string::npos ) return file_path;
	return file_path.substr( 0, dot );
}

std::string clone_name( const std::string file_path ) {
	return remove_extension( file_path ) + "_clone" + get_extension( file_path );
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
	std::string extension = get_extension( file_path );
	std::string file_name = remove_extension( file_path );
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
	std::cout << "Number of chunks: " << chunks << std::endl;
	std::cout << "Individual chunk size: " << chunk_size << std::endl;

	// Start max buffer size at arbitrarily large number

	std::cout << "Read buffer size: " << max_buffer_size << std::endl;
	long long int last = 0;
	long long int next = max_buffer_size;

	// For each chunk, do:
	for( int i = 1; i <= chunks; ++i ) {
		memset( &buffer, 0, sizeof( buffer ) );

		// Calculate the end, in bytes, of the current chunk
		long long int current_end = ( ( i * size ) / chunks );
		std::ostringstream out_file_path;
		out_file_path << file_name << i << extension;
		std::string out_file_name( out_file_path.str() );
		paths.push_back( out_file_name );
		std::ofstream out_file;
		out_file.open( out_file_name, std::ios_base::binary | std::ios::out );

		if( out_file.is_open() ) {
			out_file.seekp(0, std::ios_base::beg);
			while ( next < current_end && last < current_end ) {
				file.read( buffer, max_buffer_size );
				out_file.write( buffer, sizeof( buffer ) );
				memset( &buffer, 0, sizeof( buffer ) );
				last = file.tellg();
				next += max_buffer_size;
			}
			memset( &buffer, 0, sizeof( buffer ) );

			if( current_end - last > 0 ) {
				last = file.tellg();
				while( last != current_end ) {
					file.read( buffer, sizeof( char ) );
					out_file.write( buffer, sizeof( char ) );
					last = file.tellg();
				}
			}
			next += max_buffer_size;
		}
		out_file.close();
		std::cout << '\r' << "Bytes chunked " << last << std::flush;
	}
	std::cout << '\n' << std::endl;
	return paths;	
}

void combine_files( const std::vector<std::string>& paths, const std::string out_path ) {
	std::ofstream output( out_path, std::ios_base::binary | std::ios::out );

	for( const auto& path : paths ) {
		std::ifstream file( path, std::ios_base::binary );
		output << file.rdbuf();
	}
}

int test_chunks( const std::string& file_path, const int chunks ) {
	auto begin = std::chrono::high_resolution_clock::now();
	std::vector<std::string> paths = chunk_file( chunks, file_path );
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>( end - begin ).count() << " s" << std::endl;
	std::string new_name = clone_name( file_path );
	combine_files( paths, new_name );


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
	test_chunks("/Users/matthewmoore/Desktop/silicon_valley.mov", 5);
    return 0;
}



