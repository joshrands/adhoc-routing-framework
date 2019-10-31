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
LDFLAGS = -g -pthread -I../socket/ -I../aodv/ -Wall
LIBFLAGS = -cvq 

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libhardware.a
OBJ_LIB_FILES = hardware_aodv.o ../aodv/*.o ../socket/*.o #../socket/udp_socket.o ../socket/endpoint.o $(../socket/*.o) $(../aodv/*.o)

STATIC_LIBRARIES = ../socket/libsocket.a ../aodv/libaodv.a

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES}
INC_FILES = hardware_aodv.h #../aodv/*.h ../socket/*.h #endpoint.h safe_queue.h socket.h udp_socket.h message.h 

SUBDIRS = ../aodv ../socket

#
# Any libraries we might need.
#
LIBRARYS = -lpthread

${TARGET} : ${OBJ_FILES}
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}
	rm test.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES} 
	rm "\*.o"

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $< 

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core ${TARGET} hardware_aodv.o ${LIB_NAME}
