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

#include "messages.pb.h"

namespace {
    // Change your file paths as necessary
    const char* take_picture = "raspistill -o /home/pi/video_stream/motion_pic.jpg";
    const char* take_video = "raspivid -o /home/pi/video_stream/video.h264 -t 30000 -d";
    const char* remove_video = "rm /home/pi/video_stream/video.h264";
    const char* remove_pic = "rm /home/pi/video_stream/motion_pic.jpg";
    const std::string video_path = "/home/pi/video_stream/video.h264";
    const std::string pic_path = "/home/pi/video_stream/motion_pic.jpg";
    const int block_size = 4096;
    bool streaming = false;
    zmq::context_t context{ 1 };
    zmq::socket_t request_socket{ context, ZMQ_REQ };
}

long long int get_file_size( std::ifstream& file ) {
    file.seekg( 0, std::ios::end );
    long long int file_size = file.tellg();
    file.seekg( 0, std::ios::beg );

    return file_size;
}


zmq::message_t create_container( long long int file_size ) {
    gpb::Request file_receipt{};
    file_receipt.set_bytes( file_size );

    int size = file_receipt.ByteSize();
    void* buffer = malloc( size );

    file_receipt.SerializeToArray( buffer, size );

    zmq::message_t container( size );
    memcpy( container.data(), buffer, size );
    free( buffer );

    return container;
}

gpb::Reply unwrap_container( zmq::message_t& message ) {
    gpb::Reply server_reply{};
    server_reply.ParseFromArray( message.data(), sizeof( message ) );
    
    return server_reply; 
}


void react_to_motion( zmq::socket_t& socket ) {
    // Buffer to hold video file contents
    std::vector<char> buffer( block_size, 0 );

    // Start recording video
    system( take_video );
    std::cout << "Finished taking video" << std::endl;

    // Open the video file as a binary file
    std::ifstream file( video_path.c_str(), std::ios_base::binary | std::ios::ate );

    long long int file_size = get_file_size( file );
    double total_sent = 0;

    // Read data from file 4096 bytes at a time and send to server
    while( file.read( buffer.data(), buffer.size() ) && !file.eof() ) {
	    buffer.shrink_to_fit();
	    total_sent += buffer.size();
	    zmq::message_t encoded_message( buffer.size() );
	    memcpy( encoded_message.data(), buffer.data(), buffer.size() );
	    if( !socket.send( encoded_message ) ) {
	        std::cout << "Send failed!" << std::endl;
	        break;
	    } else {
	        std::cout << '\r' << "Sent: " << total_sent << std::flush;
	    }
    }


    // Look into ways to buffer the videos and handle this in separate thread
    // TODO: Separate function /////////////////////////////////////////////
    request_socket.send( create_container( file_size ) );

    zmq::message_t receive_buffer{};

    request_socket.recv( &receive_buffer );

    gpb::Reply reply = unwrap_container( receive_buffer );

    if( reply.bytes() == file_size )
        streaming = false;
    else
        // TODO Handle error 
    /////////////////////////////////////////////////////////////////////////


    std::cout << "Finished sending video" << std::endl;
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
    request_socket.connect( "tcp://192.168.1.3:5556" );
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
