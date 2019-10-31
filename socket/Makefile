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
LIB = ar
CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK 
LDFLAGS = -g -pthread -fsanitize=address
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libsocket.a 
OBJ_LIB_FILES = endpoint.o socket.o udp_socket.o  message.o 

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = endpoint.h socket.h udp_socket.h message.h safe_queue.h

# Any libraries we might need.
LIBRARYS = -lpthread


${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}
	rm test.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	rm -f core ${TARGET} ${OBJ_FILES} ${LIB_NAME}
	rm -f "*.o"
