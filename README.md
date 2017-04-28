# video_stream [![Build Status](https://travis-ci.org/moorem27/video_stream.svg?branch=development)](https://travis-ci.org/moorem27/video_stream)

# Development Dependencies
| server       | client          |
| ------------- |:-------------:|
| gcc 5.4.0+      | gcc 5.4.0+ |
| gcc-c++ 5.4.0+      | gcc-c++ 5.4.0+ |
| make 4.1+ | make 4.1+ |
| cmake 3.1.0+ | cmake 3.1.0+ |
| zeromq 4.2.2+ | zeromq 4.2.2+ |
| cppzmq | cppzmq |
| protobuf-cpp 3.2.0 | protobuf-cpp 3.2.0 |
| ffmpeg | wiringPi |

## Set up on Raspberry Pi
The image below is a useful reference for the pin layout and location of the camera module on the Raspberry Pi.
![](images/rp_pinout.png?raw=true)
We will be using the WiringPi library to control our motion sensor (wiringpi.com), and will initialize wiringPi to use
the BCM GPIO pins directly instead of using a virtual mapping. In our project GPIO pin 7 will be used for output from
the sensor, power will be to 5V and ground can be any pin marked as ground (though for simplicity we keep everything on the same column).

## How to Run Application
```bash
# build & run server
# NOTE: this will only build on x86/amd architectures
docker-compose -d -f server/docker-compose.yml

# build & run client
# NOTE: this will only build on arm architectures
docker-compose -d -f client/docker-compose.yml
```

## How to Setup Development Environment
```bash
vagrant up # sit back and relax because this will take a while
```
## How to Run Application in Development Environment
```bash
vagrant up
vagrant ssh
cd /vagrant
mkdir build
cd build
cmake ..
make
```
