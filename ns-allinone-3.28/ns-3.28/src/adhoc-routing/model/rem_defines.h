#ifndef REMDEFINES_H
#define REMDEFINES_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "defines.h"
#include "aodv_defines.h"

#define REM_DEBUG   		DEBUG && 0
#define BATTERY_DEBUG       DEBUG && 1
#define RSS_DEBUG           DEBUG && 1
#define HELLO_DEBUG         DEBUG && 1

#endif
