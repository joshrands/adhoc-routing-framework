#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG 0

#include <stdint.h>

#define IP_ADDR			    uint32_t

static int ROUTING_PORT =       5555;
static int DATA_PORT =          5432;
static int MONITOR_PORT =       5123;
static int LOCAL_DATA_UPDATE_RATE_MS = 500; // 500 milliseconds

#endif