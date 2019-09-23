#include "aodv_rerr.h"

// default constructor
RERRHelper::RERRHelper()
{

}

RERRHelper::RERRHelper(IP_ADDR ip, AODVRoutingTable* table, uint32_t* seqNum)
{

}

bool RERRHelper::shouldGenerateRERRFromBrokenLink(IP_ADDR brokenLink)
{

}

bool RERRHelper::attemptLocalRepair(IP_ADDR brokenLink)
{

}

rerrPacket RERRHelper::createRERR(const IP_ADDR dest)
{

}

void RERRHelper::forwardRERR(const rerrPacket receivedRERR)
{

}

char* RERRHelper::createRERRBuffer(const rerrPacket rerr)
{

}

rerrPacket RERRHelper::readRERRBuffer(char* buffer)
{
    
}
