# video_stream

# Dependencies
1. gcc 5.4.0+
2. make 4.1+
3. cmake 3.1.0+
4. zeromq-4.2.2+
5. cppzmq

## How to Build Application
TODO: make this into an ansible playbook
```bash
vagrant up
vagrant ssh
cd /vagrant
mkdir build
cd build
cmake ..
make
./server &
./client &
```

## Change ZMQ version
In order to change the installed version of ZMQ, change the version in the [defaults/main.yml](https://github.com/moorem27/video_stream/blob/master/roles/packages/defaults/main.yml) file:
```yaml
# roles/packages/defaults/main.yml
---
zmq_version: <desired-version-here>
zmq_full_name: "zeromq-{{ zmq_version }}"
```