#ifndef DEFINES_H
#define DEFINES_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_ADDR			    uint32_t
#define BROADCAST           "255.255.255.255"
#define HEADER_SIZE         9

#define DEBUG               1

#define AODV_PRINT_PACKET   1
#define AODV_LOG_OUTPUT	    1
#define AODV_DEBUG		    DEBUG && 1 
#define RREQ_DEBUG		    DEBUG && 1
#define RREP_DEBUG		    DEBUG && 1
#define RERR_DEBUG          DEBUG && 1
#define TABLE_DEBUG         DEBUG && 1
#define MONITOR_DEBUG       DEBUG && 1

#endif