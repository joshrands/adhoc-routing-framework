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
INC_DIRS = -I../adhoc

CXXFLAGS = -g -fPIC -pthread -std=c++11 -DBOOST_LOG_DYN_LINK -Wall ${INC_DIRS}
LIBFLAGS = ru

#
# You should be able to add object files here without changing anything else
#
LIB_NAME = libadhoc.a
OBJ_LIB_FILES = adhoc_routing.o port.o ../aodv/*.o ../monitoring/*.o
OBJ_FILES = ${OBJ_LIB_FILES}

INC_FILES = adhoc_routing.h port.h

SUBDIRS = ../aodv ../monitoring

#
# Any libraries we might need.
#
LIBRARYS = -lpthread

${LIB_NAME} : ${OBJ_FILES}
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
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
