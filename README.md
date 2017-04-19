# video_stream [![Build Status](https://travis-ci.org/moorem27/video_stream.svg?branch=master)](https://travis-ci.org/moorem27/video_stream)

test

# Development Dependencies
1. gcc 5.4.0+
2. make 4.1+
3. cmake 3.1.0+
4. zeromq-4.2.2+
5. cppzmq
6. WiringPi
7. Docker

## Set up on Raspberry Pi
The image below is a useful reference for the pin layout and location of the camera module on the Raspberry Pi.  
![](images/rp_pinout.png?raw=true)    
We will be using the WiringPi library to control our motion sensor (wiringpi.com), and will initialize wiringPi to use  
the BCM GPIO pins directly instead of using a virtual mapping. In our project GPIO pin 7 will be used for output from  
the sensor, power will be to 5V and ground can be any pin marked as ground (though for simplicity we keep everything on the same column). 

## How to Run Application
```bash
docker build -t videostream:latest .

# Ensures that the project and /sys directories are exposed to the Docker container
#
# from larsks @ http://stackoverflow.com/questions/30059784/docker-access-to-raspberry-pi-gpio-pins
# This would expose /sys on the host as /sys inside the container, and you would have access to the
# /sys/class/gpio hierarchy.
#
# If you were using code that access the GPIO pins without using the sysfs interface you would need to
# expose whatever device node it is using inside the container, possibly with something like the
# --device argument to docker run.
docker run -v /<videostream_project_dir>:/videostream -v /sys:/sys -it videostream /bin/bash
```

## How to Setup Development Environment
```bash
vagrant up
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
