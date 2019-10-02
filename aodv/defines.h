#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_ADDR			uint32_t
#define BROADCAST       "255.255.255.255"

#define AODV_DEBUG		    1
#define AODV_PRINT_PACKET   1
#define AODV_LOG_OUTPUT	    1

#define RREQ_DEBUG		    1
#define RREP_DEBUG		    1
#define RERR_DEBUG          1
