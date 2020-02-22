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
INC_DIRS = -I../ -I../aodv -I../rem -I../adhoc -I../hello_monitor

CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK ${INC_DIRS}
LDFLAGS = -g -std=c++11 -pthread ${INC_DIRS}
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
LIB_NAME = libsim.a
OBJ_LIB_FILES = rem_sim.o aodv_sim.o ../aodv/*.o ../adhoc/*.o ../rem/*.o

OBJ_FILES = ${OBJ_LIB_FILES} 
INC_FILES = rem_sim.h aodv_sim.h

SUBDIRS = ../aodv ../adhoc ../rem

#
# Any libraries we might need.
#
LIBRARYS = -lpthread

${LIB_NAME} : ${OBJ_FILES}
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS}  -o $@ $< 

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core ${LIB_NAME} ${OBJ_FILES}
	rm -f "*.o"