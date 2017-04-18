# For accessing Raspberry Pi's GPIO pins: http://stackoverflow.com/questions/30059784/docker-access-to-raspberry-pi-gpio-pins

FROM resin/rpi-raspbian:latest
MAINTAINER Brandon Soto (brandon.soto09@gmail.com)

ADD https://github.com/zeromq/libzmq/releases/download/v4.2.2/zeromq-4.2.2.tar.gz /tmp
ADD https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp /usr/include
ADD https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq_addon.hpp /usr/include

RUN \
   apt-get -y update && \
   apt-get -y install vim cmake build-essential && \
   cd /tmp && \
   tar xvf zeromq-4.2.2.tar.gz && \
   cd zeromq-4.2.2 && ./configure && make -j4 && make install && \
   rm -rf /var/lib/apt/lists/* && \
   rm -rf /tmp/*

EXPOSE 4242
