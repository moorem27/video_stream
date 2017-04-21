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
// bobo comment

namespace {
    // Change your file paths as necessary
    const char* take_picture = "raspistill -o /home/pi/samsung/motion_pic.jpg";
    const char* take_video = "raspivid -o /home/pi/samsung/video.h264 -t 30000 -d";
    const char* remove_video = "rm /home/pi/samsung/video.h264";
    const char* remove_pic = "rm /home/pi/samsung/motion_pic.jpg";
    const std::string video_path = "/home/pi/samsung/video.h264";
    const std::string pic_path = "/home/pi/samsung/motion_pic.jpg";
    const int send_size = 163840;
    std::string server_IP{};
    bool streaming = false;
}

int create_connection() {
    struct sockaddr_in server_connection{};
    int file_descriptor = socket( PF_INET, SOCK_STREAM, 0 );

    memset( &server_connection, 0, sizeof( server_connection ) );
    server_connection.sin_family = AF_INET;
    server_connection.sin_addr.s_addr = inet_addr( server_IP.c_str() ); // Server address
    server_connection.sin_port = htons( 8888 );

    int timeout = 0;
    while( ++timeout < 100 ) {
        connect( file_descriptor, reinterpret_cast<struct sockaddr*>( &server_connection ), sizeof( server_connection ) );
    }

    setsockopt( file_descriptor, SOL_SOCKET, SO_SNDBUF, &send_size, sizeof( send_size ) );
    return file_descriptor;
}


// TODO: Refactor and use file chunking and networking libraries
void react_to_motion( const int send_fd ) {
    std::vector<char> buffer( send_size, 0 );
    system( take_video );
    std::cout << "Finished taking video" << std::endl;
    std::ifstream file( video_path.c_str(), std::ios_base::binary | std::ios::ate );
    file.seekg( 0, std::ios::beg );

    while( file.read( buffer.data(), buffer.size() ) ) {
        if( send( send_fd, static_cast<void *>( buffer.data() ), buffer.size(), 0 ) < 0 )
            std::cout << "Send failed" << std::endl;
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
    const int send_fd = create_connection();
//    std::cout << "send_fd " << send_fd << std::endl;
//    std::this_thread::sleep_for( std::chrono::seconds( 15 ) );
//
//
//    while( true ) {
//        if( !streaming ) {
//            if ( digitalRead( 7 ) ) {
//                streaming = true;
//                std::cout << "Motion detected!" << std::endl;
//                react_to_motion( send_fd );
//                break;
//            }
//            std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
//        }
//    }
    return 0;
}
