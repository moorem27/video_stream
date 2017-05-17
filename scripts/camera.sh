#!/bin/bash

# Run this script on the Raspberry Pi to stream video to port 8554 over rtp
# (VLC and raspivid need to be installed)

raspivid -o - -t 0 -hf -w 640 -h 360 -fps 25 | cvlc -vvv stream:///dev/stdin --sout '#rtp{sdp=rtsp://:8554}' :demux=h264

