#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Josh Rands on 9/2/19
#  Modified by Zachary Smeton on 10/1/19
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
LIB = ar
CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK -Wall
LDFLAGS = -g -pthread -I../socket/ -Wall
LIBFLAGS = -cvq 

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libhardware.a
OBJ_LIB_FILES = hardware_aodv.o ../socket/endpoint.o ../socket/socket.o ../socket/udp_socket.o ../socket/message.o 

STATIC_LIBRARIES = ../aodv/libaodv.a ../socket/libsocket.a

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = hardware_aodv.h #endpoint.h safe_queue.h socket.h udp_socket.h message.h 

#
# Any libraries we might need.
#
LIBRARYS = -lpthread

${TARGET} : ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS} ${STATIC_LIBRARIES} 
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES} 

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core ${TARGET} ${OBJ_FILES} ${LIB_NAME}
