#!/bin/bash

(realpath --v > /dev/null) || (echo 'realpath does not exist. Exiting now.' && exit 1)

BASEDIR=`realpath $0`
BASEDIR=`dirname $BASEDIR`

docker build -f Dockerfile-client -t vs-client:latest .
docker run --privileged -p 5555:5555 -v "$BASEDIR":/videostream \
--device=/dev/vchiq --device=/dev/gpiomem -it vs-client /bin/bash
