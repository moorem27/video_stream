# This container should be run on the Raspberry Pi client

FROM brandonsoto/cppzmq:rpi
MAINTAINER Brandon Soto (brandon.soto09@gmail.com)

RUN \
   apt-get update -y -qq && \
   apt-get install -y -q vim rpi-update && \
   SKIP_WARNING=1 /usr/bin/rpi-update > /dev/null && \
   echo "/opt/vc/lib" > /etc/ld.so.conf.d/00-vmcs.conf && \
   ldconfig && \
   ln -s /opt/vc/bin/raspivid /usr/bin/raspivid && \
   ln -s /opt/vc/bin/raspistill /usr/bin/raspistill && \
   cd /opt && \
   git clone git://git.drogon.net/wiringPi && \
   cd wiringPi && \
   git pull origin && \
   /opt/wiringPi/build && \
   rm -rvf /var/lib/apt/lists/* && \
   rm -rvf /tmp/*

EXPOSE 5555