# video_stream

# Dependencies
1. gcc 5.4.0+
2. make 4.1+
3. cmake 3.1.0+
4. zeromq-4.2.2+
5. cppzmq
6. WiringPi
## Set up on Raspberry Pi
The image below is a useful reference for the pin layout and location of the camera module on the Raspberry Pi.  
![](images/rp_pinout.png?raw=true)    
We will be using the WiringPi library to control our motion sensor (wiringpi.com), and will initialize wiringPi to use  
the BCM GPIO pins directly instead of using a virtual mapping. In our project GPIO pin 7 will be used for output from  
the sensor, power will be to 5V and ground can be any pin marked as ground (though for simplicity we keep everything on  
the same column).  
## How to Build Application
TODO: make this into an ansible playbook
```bash
vagrant up
vagrant ssh
cd /vagrant
mkdir build
cd build
cmake ..
make
./server &
./client &
```

## Change ZMQ version
In order to change the installed version of ZMQ, change the version in the [defaults/main.yml](https://github.com/moorem27/video_stream/blob/development/roles/packages/defaults/main.yml) file:
```yaml
# roles/packages/defaults/main.yml
---
zmq_version: <desired-version-here>
zmq_full_name: "zeromq-{{ zmq_version }}"
```