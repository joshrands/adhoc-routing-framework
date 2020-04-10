#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Josh Rands on 9/2/19
#  Modified by Zachary Smeton on 10/1/19
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

PREFIX ?= /usr
SBINDIR ?= $(PREFIX)/sbin
MANDIR ?= $(PREFIX)/share/man
PKG_CONFIG ?= pkg-config

MKDIR ?= mkdir -p
INSTALL ?= install
CC ?= "g++"

CFLAGS ?= -std=c++17 -O2 -g -Wall -Wundef -Wno-trigraphs -fno-strict-aliasing -fno-common

LIBS += -lpcap

OBJS += ./radiotap.o
OBJS += ./radiotap_capture.o
OBJS += ./test.o

-include $(OBJS:%.o=%.d)

all: test

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

test: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)
	rm test.o

clean:
	rm -f *.o main
	rm -f *.d

.PHONY : all
