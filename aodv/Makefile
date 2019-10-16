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
LDFLAGS = -g -pthread
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libaodv.a
OBJ_LIB_FILES = RoutingProtocol.o aodv.o aodv_rreq.o aodv_rrep.o aodv_rerr.o aodv_routing_table.o send_packet.o aodv_sim.o 

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = ${TARGET}.h RoutingProtocol.h aodv.h aodv_rreq.h aodv_rrep.h aodv_rerr.h aodv_routing_table.h send_packet.h aodv_sim.h

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
