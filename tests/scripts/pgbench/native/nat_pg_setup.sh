#!/bin/bash

cd /home/lind/lamp_native/postgres/

export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
export CC=/usr/local/gcc-4.4.3/bin/gcc 
export CXX=/usr/local/gcc-4.4.3/bin/g++

./configure
make
sudo make install
sudo mkdir -m 770 /usr/local/pgsql/data/
sudo chown lind:lind /usr/local/pgsql/data

export LC_ALL=C
/usr/local/pgsql/bin/initdb --username=lind --debug --no-sync --auth=trust -E UNICODE --locale=C -D /usr/local/pgsql/data