AKEFLAGS += --no-print-directory

PREFIX ?= /usr
SBINDIR ?= $(PREFIX)/sbin
MANDIR ?= $(PREFIX)/share/man
PKG_CONFIG ?= pkg-config

MKDIR ?= mkdir -p
INSTALL ?= install
CC ?= "g++"

CFLAGS ?= -MMD -O2 -g
CFLAGS += -Wall -Wundef -Wno-trigraphs -fno-strict-aliasing -fno-common

LIBS += -lpcap

OBJS += ./radiotap.o
OBJS += ./wicap.o
OBJS += ./main.o

-include $(OBJS:%.o=%.d)

all: main

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o main

.PHONY : clean
