#!/bin/bash

# TODO Why isn't this working...FFS
gst-launch-1.0 -v udpsrc port=5000 ! gdpdepay ! rtph264depay ! avdec_h264 ! fpsdisplaysink sync=false text-overlay=false
