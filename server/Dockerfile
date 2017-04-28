# This container should be run on server
FROM brandonsoto/cppzmq
MAINTAINER Brandon Soto (brandon.soto09@gmail.com)

ADD . /src

RUN \
    apt-get -y -qq update && \
    apt-get -y -qq install ffmpeg && \
    rm -rf /var/lib/apt/lists/*

RUN echo 'building server...' && cd /src && mkdir build && cd build && cmake .. && make

CMD ["/src/build/server"]

EXPOSE 5555
