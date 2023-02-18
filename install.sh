#!/bin/bash

# install protobuffer compiler v3.11.0
# follow here to install protobuffer: https://github.com/protocolbuffers/protobuf/blob/3.11.x/src/README.md
sudo apt-get install autoconf automake libtool curl make g++ unzip
wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.11.0.zip
unzip v3.11.0.zip
cd protobuf-3.11.0
./autogen.sh
./configure
make
sudo make install
# sudo ldconfig # refresh shared library cache.
cd ..

# build RemCCG
sudo apt-get install clang-6.0 clang++-6.0
mkdir build
cd build
cmake ..
make -j4
cd ..
# you can find the `remccg` in build/src and use it to perform compiler testing
