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
OBJ_LIB_FILES = hardware/hardware_aodv.o #$(socket/*.o)
STATIC_LIBRARIES = hardware/libhardware.a 

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = 

SUBDIRS = aodv socket hardware

${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${STATIC_LIBRARIES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	

clean-all: 
	for dir in $(SUBDIRS); do \
		cd $$dir; \
		make clean; \
		cd ../; \
	done
	rm -f core ${TARGET} ${OBJ_FILES}
	rm -f "*.o"

clean:
	rm -f core ${TARGET} ${OBJ_FILES}
	rm -f "*.o"
