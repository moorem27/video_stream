#include <string.h>
#include <vector>
#include <fstream>
#include <sys/socket.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <zmq.h>
#include <zmq_addon.hpp>
#include <wiringPi.h>


namespace {
    // Change your file paths as necessary
    const char* take_picture = "raspistill -o /tmp/motion_pic.jpg";
    const char* take_video = "raspivid -o /tmp/video.h264 -t 5000 -d";
    const char* remove_video = "rm /tmp/video.h264";
    const char* remove_pic = "rm /tmp/motion_pic.jpg";
    const std::string video_path = "/tmp/video.h264";
    const std::string pic_path = "/tmp/motion_pic.jpg";
    const int block_size = 8192;
    bool streaming = false;
}

int get_file_size( std::ifstream& file ) {
    file.seekg( 0, std::ios::end );
    int file_size = file.tellg();
    file.seekg( 0, std::ios::beg );

    return file_size;
}


void react_to_motion( zmq::socket_t& socket ) {
    // Buffer to hold video file contents
    std::vector<char> buffer( block_size, 0 );

    // Start recording video
    system( take_video );
    std::cout << "Finished taking video" << std::endl;

    // Open the video file as a binary file
    std::ifstream file( video_path.c_str(), std::ios_base::binary | std::ios::ate );

    int file_size = get_file_size( file );
    std::cout << "File size: " << file_size << std::endl;
    int total_sent = 0;
    int bytes_to_read = 0;
    int read_size = block_size;

    if ( file_size < read_size ) {
	read_size = file_size;
    }

    // Read data from file chunks at a time and send to server
    while( read_size > 0 && file.read( buffer.data(), read_size ) ) {
            total_sent += read_size;

            zmq::multipart_t multipart{};
            multipart.addtyp( file_size );
            multipart.addmem( buffer.data(), read_size );

            if ( !multipart.send( socket ) ) {
                std::cout << "Send failed! at " << total_sent << std::endl;
                break;
            }

	    const int remaining_size = file_size - total_sent;

	    if ( read_size > remaining_size ) {
		read_size = remaining_size;
	    }

	    buffer.assign( block_size, 0 );
    }
    if( total_sent == file_size )
        // TODO: If the entire file was sent, we should see this print......but we don't......Why??
        std::cout << "Total sent: " << total_sent << std::endl;
    streaming = false;


    std::cout << "Finished sending video " << total_sent << std::endl;
    file.close();
    system( remove_video );
}

// TODO: Make this event based instead of polling
int main( int argc, char* argv[] ) {
    // Create address
    std::string setup = wiringPiSetupGpio() == 0 ? "Good to go" : "Set up failed";
    std::cout << setup << std::endl;
    zmq::context_t context{ 1 };
    zmq::socket_t socket{ context, ZMQ_PUSH };
    socket.bind( "tcp://*:5555" ); 
    std::this_thread::sleep_for( std::chrono::seconds( 2 ) );

    // TODO Decide when to exit loop
    while( true ) {
        if( !streaming ) {
            if ( digitalRead( 7 ) ) {
                streaming = true;
                std::cout << "Motion detected!" << std::endl;
                react_to_motion( socket );
            }
        }
	std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    }
    return 0;
}
