#!/bin/bash

# Why isn't this working FFS...

gst-launch-1.0 -e v4l2src do-timestamp=true ! h264parse ! rtph264pay config-interval=1 pt=96 ! gdppay ! udpsink host=UBUNTU.IP.GOES.HERE port=5000
