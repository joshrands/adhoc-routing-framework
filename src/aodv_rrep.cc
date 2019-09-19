#define RREP_DEBUG		1

#include "aodv_rrep.h"

#include <iostream>
#include <string.h>

using namespace std;

RREPHelper::RREPHelper()
{

}

RREPHelper::RREPHelper(IP_ADDR ip, AODVRoutingTable* table) 
{
    this->m_table = table;
    this->m_ip = ip;
}

rrepPacket RREPHelper::createRREP(const IP_ADDR dest)
{
/*
Immediately before a destination node originates a RREP in
response to a RREQ, it MUST update its own sequence number to the
maximum of its current sequence number and the destination
sequence number in the RREQ packet.
*/
}

void RREPHelper::forwardRREP(const rrepPacket receivedRREP)
{

}

char* RREPHelper::createRREPBuffer(const rrepPacket rrep)
{

}

rrepPacket RREPHelper::readRREPBuffer(char* buffer)
{

}
