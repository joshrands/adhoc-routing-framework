#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG 1
#define RPI 0

#include <stdint.h>

#define IP_ADDR			    uint32_t

#if RPI
#define BROADCAST_STR       "192.168.1.255"
#else
#define BROADCAST_STR       "255.255.255.255"
#endif

#define MY_IP_ADDR          "192.168.1.1"
#define INTERFACE_NAME      "wlan0"

#define ROUTING_PORT        5969
#define DATA_PORT           5432
#define MONITOR_PORT        5123
#define LOCAL_DATA_UPDATE_RATE_MS 500
#define HELLO_PORT          5124

#define PACKET_BUFFER_SIZE  10

#endif
