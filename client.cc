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
#include <zmq.h>
#include <zmq_addon.hpp>
#include <wiringPi.h>

namespace {
    // Change your file paths as necessary
    const char* take_picture = "raspistill -o /home/pi/samsung/motion_pic.jpg";
    const char* take_video = "raspivid -o /home/pi/samsung/video.h264 -t 30000 -d";
    const char* remove_video = "rm /home/pi/samsung/video.h264";
    const char* remove_pic = "rm /home/pi/samsung/motion_pic.jpg";
    const std::string video_path = "/home/pi/samsung/video.h264";
    const std::string pic_path = "/home/pi/samsung/motion_pic.jpg";
    const int block_size = 4096;
    std::string server_IP{};
    bool streaming = false;
}


zmq::socket_t create_socket( zmq::context_t& context ) {
    return zmq::socket_t( context, ZMQ_PUSH );    
}


void react_to_motion( zmq::socket_t& socket ) {
    // Buffer to hold video file contents
    std::vector<char> buffer( block_size, 0 );

    // Start recording video
    system( take_video );
    std::cout << "Finished taking video" << std::endl;

    // Open the video file as a binary file
    std::ifstream file( video_path.c_str(), std::ios_base::binary | std::ios::ate );
    file.seekg( 0, std::ios::beg );

    while( file.read( buffer.data(), buffer.size() ) ) {
	buffer.shrink_to_fit();
	zmq::message_t encoded_message( buffer.size() );
	memcpy( encoded_message.data(), buffer.data(), buffer.size() );
	socket.send( encoded_message );
    }
    std::cout << "Finished sending video" << std::endl;
    file.close();
    system( remove_video );
    streaming = false;
}

// TODO: Make this event based instead of polling
int main( int argc, char* argv[] ) {
    server_IP = argv[ 1 ];
    std::cout << wiringPiSetupGpio() << std::endl;
    zmq::context_t context{ 1 };
    zmq::socket_t socket{ context, ZMQ_PUSH };

    std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
    while( true ) {
        if( !streaming ) {
            if ( digitalRead( 7 ) ) {
                streaming = true;
                std::cout << "Motion detected!" << std::endl;
                react_to_motion( socket );
            }
            std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        }
    }
    return 0;
}
