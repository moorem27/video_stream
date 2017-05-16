#!/bin/bash

# Example of streaming video from raspberry pi to server using a combination of VLC and raspivid
# VLC must be installed on Pi and server

raspivid -o - -t 0 -hf -w 640 -h 360 -fps 25 | cvlc -vvv stream:///dev/stdin --sout '#rtp{sdp=rtsp://:8554}' :demux=h264

# To watch this stream on server side open VLC, press Media -> Open Networkstream and type:
# rtsp://IP.TO.THE.PI:8554/
# OR
# in the command line type:
# vlc rtsp://IP.TO.THE.PI:8554/
