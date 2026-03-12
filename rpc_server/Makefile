TARGET=rpc_server
CXX=arm-buildroot-linux-gnueabihf-g++

TOP_DIR=$(shell pwd)/../
LIBEV_DIR=${TOP_DIR}/libev/tmp/

CFLAGS=-I${LIBEV_DIR}/include 

c_files = cJSON.cpp rpc_server.cpp led.cpp dht11.cpp curtain.cpp gpio.cpp motor.cpp jsonrpc-c.c weather.cpp

LDFLAGS := ${LIBEV_DIR}/lib/libev.a -lm -lpthread -lcurl

all:
	${CXX} ${CFLAGS} -o ${TARGET} ${c_files} ${LDFLAGS}

clean:
	rm -f *.o ${TARGET}
