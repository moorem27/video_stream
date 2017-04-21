j.PHONY : all build run clean
all: build

# set build context
context:
	docker run --rm --privileged multiarch/qemu-user-static:register --reset

build: clean client server

client: context
	docker build -t vs-client -f Dockerfile-client .

server:
	docker build -t vs-server -f Dockerfile-server .

stop-server:
	docker rm vs-server &> /dev/null
	docker stop vs-server &> /dev/null

stop-client:
	docker rm vs-client &> /dev/null
	docker stop vs-client &> /dev/null

clean: clean-server clean-client

clean-server: stop-server
	docker rmi vs-server &> /dev/null

clean-client: stop-client
	docker rmi vs-client &> /dev/null

