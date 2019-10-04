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
CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK -Wall
LDFLAGS = -g -pthread -I../socket/ -Wall

#
# You should be able to add object files here without changing anything else
#
TARGET = test
OBJ_FILES = ${TARGET}.o endpoint.o socket.o udp_socket.o 
INC_FILES = endpoint.h safe_queue.h socket.h udp_socket.h message.h 

#
# Any libraries we might need.
#
LIBRARYS = -lpthread


${TARGET}.a : ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core ${TARGET} ${OBJ_FILES}
