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

// Maximum bandwidth in MB. To change bandwidth, change this constant.
#define BANDWIDTH_MB		1
#define BITS_PER_MB			8000000
#define BANDWIDTH_BITS_MAX	BANDWIDTH_MB * BITS_PER_MB

/*
 * Reference bandwidth, used to calculate distance based quadratic signal loss.
 * This assumes 10% of bandwidth can be used at the max distance of the signal,
 * and that you will have 100% of the signal at 10% of the max distance.
 */
#define BANDWIDTH_0			BANDWIDTH_BITS_MAX * 0.1
// Approximate "max" distance of signal, in meters. Update as needed.
#define BANDWIDTH_DIST_MAX	200.0
#define SQRT_OF_10_PERC		0.316227766
#define BANDWIDTH_DIST_0	BANDWIDTH_DIST_MAX * SQRT_OF_10_PERC

#define PACKET_BUFFER_SIZE  10

#endif
