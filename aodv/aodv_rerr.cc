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

rerrPacket RERRHelper::createRERR(const IP_ADDR destIP, const IP_ADDR origIP)
{
    if (RERR_DEBUG)
        cout << "Generating Route Error packet for destination " << getStringFromIp(destIP) << endl;

    rerrPacket rerr;

    // populate data from unreachable 
    // start with dest count of 1
    rerr.destCount = 1;
    rerr.unreachableIP = destIP;
    rerr.unreachableSeqNum = this->m_pTable->getDestSequenceNumber(destIP);
    rerr.origIP = origIP;

	return rerr;
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
