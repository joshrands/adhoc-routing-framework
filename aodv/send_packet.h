#pragma once 

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

#define DEBUG    0 

using namespace std;

int sendBuffer(char* buffer, int length, IP_ADDR dest, int port);
IP_ADDR getLastSource();
void setLastSource(IP_ADDR lastSource);
uint32_t getGlobalPacketCount();
void incrementGlobalPacketCount();
