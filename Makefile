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
INC_DIRS = -I./aodv -I./monitoring -I./hardware -I./socket -I./adhoc -I./ 

CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK ${INC_DIRS}
LDFLAGS = -g -pthread ${INC_DIRS}
LIBFLAGS = -cvq

# You should be able to add object files here without changing anything else
TARGET = do-adhoc
OBJ_LIB_FILES = hardware/hardware_aodv.o

STATIC_LIBRARIES = hardware/libhardware.a adhoc/libadhoc.a

OBJ_FILES = ${OBJ_LIB_FILES} 
INC_FILES = 

SUBDIRS = aodv socket hardware adhoc

${TARGET}: #${OBJ_FILES}
	${CXX} -c ${CXXFLAGS} ${TARGET}.cc -o ${TARGET}.o
	${LD} ${LDFLAGS} ${OBJ_FILES} ${TARGET}.o -o $@ ${STATIC_LIBRARIES}

test: 
	make clean	
	${CXX} -c ${CXXFLAGS} test.cc -o test.o
	${LD} ${LDFLAGS} test.o aodv/*.o adhoc/*.o -o test 
#	./test

test-all:
	make all 
	for dir in $(SUBDIRS); do \
		cd $$dir; \
		./test; \
		cd ../; \
	done
	make clean
	make test

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
