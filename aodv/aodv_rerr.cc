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

    return false;
}

bool RERRHelper::attemptLocalRepair(IP_ADDR brokenLink)
{

    return false;
}

rerrPacket RERRHelper::createRERR(const IP_ADDR dest)
{

    rerrPacket packet;

		return packet;
}

void RERRHelper::forwardRERR(const rerrPacket receivedRERR)
{

}

char* RERRHelper::createRERRBuffer(const rerrPacket rerr)
{
    char* buffer = (char*)(malloc(sizeof(rerr)));

    return buffer;
}

rerrPacket RERRHelper::readRERRBuffer(char* buffer)
{
   rerrPacket rerr;

   return rerr; 
}
