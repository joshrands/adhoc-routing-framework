#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "defines.h"
#include "aodv_defines.h"

#define IP_ADDR			    uint32_t

#define REM_DEBUG   		DEBUG && 1
#define BATTERY_DEBUG       DEBUG && 1
#define RSS_DEBUG           DEBUG && 1
