#!/bin/bash

wget http://www.digip.org/jansson/releases/jansson-2.11.tar.gz
tar -xvf jansson-2.11.tar.gz
cd jansson-2.11
./configure
make
make check
make install
rm jansson-2.11.tar.gz