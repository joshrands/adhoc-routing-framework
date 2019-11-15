#ifndef AODV_DEFINES_H
#define AODV_DEFINES_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../defines.h"

#define BROADCAST           "255.255.255.255"
#define HEADER_SIZE         9

#define GLOBAL_AODV_DEBUG   1

#define AODV_PRINT_PACKET   1 && GLOBAL_AODV_DEBUG
#define AODV_LOG_OUTPUT	    1 && GLOBAL_AODV_DEBUG
#define AODV_DEBUG		    DEBUG && 1 && GLOBAL_AODV_DEBUG
#define RREQ_DEBUG		    DEBUG && 1 && GLOBAL_AODV_DEBUG
#define RREP_DEBUG		    DEBUG && 1 && GLOBAL_AODV_DEBUG
#define RERR_DEBUG          DEBUG && 1 && GLOBAL_AODV_DEBUG
#define TABLE_DEBUG         DEBUG && 1 && GLOBAL_AODV_DEBUG
#define MONITOR_DEBUG       DEBUG && 1 && GLOBAL_AODV_DEBUG

#endif