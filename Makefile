#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Josh Rands on 10/4/19
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
LIB = ar
CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK
LDFLAGS = -g -pthread
LIBFLAGS = -cvq

# You should be able to add object files here without changing anything else
TARGET = do-adhoc
OBJ_LIB_FILES = hardware/hardware_aodv.o adhoc_routing.o #$(socket/*.o)

STATIC_LIBRARIES = hardware/libhardware.a monitoring/libmonitoring.a 

OBJ_FILES = ${OBJ_LIB_FILES} 
INC_FILES = adhoc_routing.h

SUBDIRS = aodv socket hardware

${TARGET}: #${OBJ_FILES}
	${CXX} -c ${CXXFLAGS} adhoc_routing.cc -o adhoc_routing.o
	${CXX} -c ${CXXFLAGS} ${TARGET}.cc -o ${TARGET}.o
	${LD} ${LDFLAGS} ${OBJ_FILES} ${TARGET}.o -o $@ ${STATIC_LIBRARIES}

test: 
	${CXX} -c ${CXXFLAGS} adhoc_routing.cc -o adhoc_routing.o 	
	${CXX} -c ${CXXFLAGS} test.cc -o test.o
	${LD} ${LDFLAGS} adhoc_routing.o test.o aodv/*.o monitoring/*.o -o test 
	./test

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} do-adhoc.cc -o $@ $<

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	make 

clean-all: 
	for dir in $(SUBDIRS); do \
		cd $$dir; \
		make clean; \
		cd ../; \
	done
	make clean

clean:
	rm -f core ${TARGET} test
	rm -f *.o
#	rm -f "*.o"
