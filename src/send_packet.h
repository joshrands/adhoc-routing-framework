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

#define DEBUG     1

using namespace std;

void sendBuffer(char* buffer, int length, IP_ADDR source, IP_ADDR dest);
IP_ADDR getLastSource();
uint32_t getGlobalPacketCount();
