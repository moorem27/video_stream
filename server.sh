#!/bin/bash

(realpath --v > /dev/null) || (echo 'realpath does not exist. Exiting now.' && exit 1)

BASEDIR=`realpath $0`
BASEDIR=`dirname $BASEDIR`

echo "$BASEDIR"

docker build -f Dockerfile-server -t vs-server:latest .
docker run -v "$BASEDIR":/videostream -it vs-server