#!/bin/bash
# update git repository
git pull

# build and install debug version of server
cmake -DCMAKE_BUILD_TYPE=Debug
make
make install

# build and install release version of server
cmake -DCMAKE_BUILD_TYPE=Release
make
make install
