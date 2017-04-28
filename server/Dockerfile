# This container should be run on server

FROM brandonsoto/cppzmq
MAINTAINER Brandon Soto (brandon.soto09@gmail.com)

RUN \
   apt-get -y -q update && \
   apt-get -y -q install vim ffmpeg && \
   rm -rvf /var/lib/apt/lists/*

EXPOSE 5555
