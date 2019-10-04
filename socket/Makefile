#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Josh Rands on 9/2/19
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
<<<<<<< HEAD
LIB = ar
CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK
=======
CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK 
>>>>>>> 04221e5e53f5196cf2bf92ea1562bb05f0e0349b
LDFLAGS = -g -pthread
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libsocket.a 
OBJ_LIB_FILES = endpoint.o socket.o udp_socket.o  message.o

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = endpoint.h safe_queue.h socket.h udp_socket.h message.h 

# Any libraries we might need.
LIBRARYS = -lpthread


${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	rm -f core ${TARGET} ${OBJ_FILES} ${LIB_NAME}