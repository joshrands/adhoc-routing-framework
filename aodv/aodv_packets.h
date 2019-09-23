#pragma once 

/*********************************
 * aodv_packets.h
 *
 * Custom c++ implementation of the aodv packets  
 * 
 * Author: Josh Rands
 * Date: 9/18/2019
 ********************************/

#include <stdint.h>
#include <endian.h>

/* RREQ Flags: */
#define RREQ_JOIN          0x1
#define RREQ_REPAIR        0x2
#define RREQ_GRATUITOUS    0x4
#define RREQ_DEST_ONLY     0x8

struct rreqPacket
{
	uint8_t type = 0x01;

#if defined(__LITTLE_ENDIAN)
    uint8_t res1:3;
	uint8_t u:1; // Unknown sequence number flag 
    uint8_t d:1; // Destination only respond 
    uint8_t g:1; // Gratuitous RREP flag
    uint8_t r:1; // Repair flag
    uint8_t j:1; // Join flag
#elif defined(__BIG_ENDIAN)
    uint8_t j:1;
    uint8_t r:1;
    uint8_t g:1;
    uint8_t d:1;
    uint8_t res1:4;
#else
#error "Adjust your <bits/endian.h> defines"
#endif
    uint8_t res2; // reserved flag space 

	uint8_t hopCount;
	uint32_t rreqID;
	uint32_t destIP;
	uint32_t destSeqNum;
	uint32_t origIP;
	uint32_t origSeqNum;
};

/* RREP Flags: */
#define RREP_ACK       0x1
#define RREP_REPAIR    0x2

struct rrepPacket
{
    uint8_t type = 0x02;
#if defined(__LITTLE_ENDIAN)
    uint16_t res1:6;
    uint16_t a:1;
    uint16_t r:1;
    uint16_t prefix:5;
    uint16_t res2:3;
#elif defined(__BIG_ENDIAN)
    uint16_t r:1;
    uint16_t a:1;
    uint16_t res1:6;
    uint16_t res2:3;
    uint16_t prefix:5;
#else
#error "Adjust your <bits/endian.h> defines"
#endif
    uint8_t hopCount;
    uint32_t destIP;
    uint32_t destSeqNum;
    uint32_t origIP;
    uint32_t lifetime;
};
