# This container should be run on server

FROM brandonsoto/cppzmq
MAINTAINER Brandon Soto (brandon.soto09@gmail.com)

ADD . /src

RUN \
   apt-get -y -q update && \
   apt-get -y -q install vim ffmpeg && \
   rm -rf /var/lib/apt/lists/*

RUN echo 'building server...' && cd /src && mkdir build && cd build && cmake .. && make

EXPOSE 5555
