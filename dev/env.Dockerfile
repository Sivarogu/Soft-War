from node:carbon-stretch

run npm i -g concurrently nodemon webpack typescript

WORKDIR /zmq
RUN git clone --quiet https://github.com/zeromq/libzmq.git libzmq
WORKDIR /zmq/libzmq
RUN ./autogen.sh 2> /dev/null
RUN ./configure --quiet --without-docs
RUN make
RUN make install
RUN ldconfig

WORKDIR /home/zmq
RUN git clone --quiet https://github.com/zeromq/czmq.git czmq
WORKDIR /home/zmq/czmq
RUN ./autogen.sh 2> /dev/null
RUN ./configure --quiet --without-docs
RUN make
RUN make install
RUN ldconfig

workdir /home/ljansson
run wget http://www.digip.org/jansson/releases/jansson-2.11.tar.gz
run tar -xvf jansson-2.11.tar.gz
workdir /home/ljansson/jansson-2.11
run ls
run ./configure --prefix=/usr/
run make
run make check
run make install
workdir /home/ljansson
run rm jansson-2.11.tar.gz

run npm i -g webpack-cli

workdir /app
