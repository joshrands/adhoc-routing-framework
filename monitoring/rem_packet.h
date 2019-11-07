/*********************************
 * rem_packet.h
 *
 * C++ implementation of the rem network packet  
 * Formerly known as ModelInfoPacket***
 *  
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

#include <stdint.h>

using namespace std; 

struct REMModelPacket
{
    uint8_t type;
    IP_ADDR parentIp;
    uint32_t timestamp; // timestamp of packet
    uint16_t timeToLive; // time to live of model in minutes

    float mu;
    float beta;
    float sigma;
}; 
