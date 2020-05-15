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
INC_DIRS = -I../ -I../aodv -I../adhoc -I../data_structures

CXXFLAGS = -g -fPIC -pthread -std=c++17 -DBOOST_LOG_DYN_LINK ${INC_DIRS}
LDFLAGS = -g -std=c++17 -pthread ${INC_DIRS}
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = librem.a
OBJ_LIB_FILES = rem.o rem_model.o ../aodv/*.o ../adhoc/*.o

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = ${TARGET}.h rem.h rem_model.h rem_packet.h rem_parameters.h monitor_defines.h

SUBDIRS = ../aodv ../adhoc

all: ${TARGET}

${TARGET}: ${OBJ_FILES}
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}
	rm test.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

run:
	make 
	./test

clean:
	rm -f "*.o"
	rm -f core ${TARGET} ${OBJ_FILES} ${LIB_NAME}
