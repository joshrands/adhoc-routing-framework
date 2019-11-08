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
LDFLAGS = -g -std=c++11 -pthread
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libmonitoring.a
OBJ_LIB_FILES = rem.o rem_model.o network_monitor.o ../aodv/RoutingProtocol.o

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = ${TARGET}.h rem.h rem_model.h rem_packet.h rem_parameters.h monitor_defines.h network_monitor.h ../aodv/RoutingProtocol.h ../defines.h

${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}
	rm test.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

run:
	make 
	./test

clean:
	rm -f core ${TARGET} ${OBJ_FILES} ${LIB_NAME}
