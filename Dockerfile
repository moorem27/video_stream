# This container should be run on the Raspberry Pi client

FROM brandonsoto/cppapp:rpi
MAINTAINER Brandon Soto (brandon.soto09@gmail.com)

ADD https://github.com/zeromq/libzmq/releases/download/v4.2.2/zeromq-4.2.2.tar.gz /tmp
ADD https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp /usr/include
ADD https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq_addon.hpp /usr/include

RUN \
   cd /tmp && \
   tar xvf zeromq-4.2.2.tar.gz && \
   cd zeromq-4.2.2 && ./configure && make -j4 && make install && \
   cd /opt && \
   git clone git://git.drogon.net/wiringPi && \
   cd wiringPi && \
   git pull origin && \
   /opt/wiringPi/build && \
   rm -rvf /var/lib/apt/lists/* && \
   rm -rvf /tmp/*

EXPOSE 4242
