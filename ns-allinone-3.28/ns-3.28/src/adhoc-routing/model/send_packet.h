#ifndef SENDPACKET_H
#define SENDPACKET_H

/*********************************
 * send_packet.h
 *
 * Interface for sending packets used by routing protocols 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

#include "RoutingProtocol.h"

#include <iostream>

using namespace std;

int sendBuffer(char* buffer, int length, IP_ADDR dest, int port);
IP_ADDR getLastSource();
void setLastSource(IP_ADDR lastSource);
IP_ADDR getLastReceive();
void setLastReceive(IP_ADDR lastReceive);
uint32_t getGlobalPacketCount();
void incrementGlobalPacketCount();

#endif