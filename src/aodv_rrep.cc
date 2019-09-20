#define RREP_DEBUG		1

#include "aodv_rrep.h"

#include <iostream>
#include <string.h>

using namespace std;

RREPHelper::RREPHelper()
{

}

RREPHelper::RREPHelper(IP_ADDR ip, AODVRoutingTable* table, uint32_t* seqNum) 
{
    this->m_pTable = table;
    this->m_ip = ip;
    this->m_pSequenceNum = seqNum;
}

rrepPacket RREPHelper::createRREPFromRREQ(rreqPacket rreq)
{
	rrepPacket rrep;    

    // populate fields of rrep
    rrep.type = 0x02;
    rrep.destIP = rreq.destIP;
    // is this node the destination? 
    if (rreq.destIP == this->m_ip)
    {
        // yes, copy this sequence number in
//        if (rreq.origSeqNum == this->m_seq)
        // TODO: increment this node sequence number if equal to orig sequence number 

        rrep.hopCount = 0x00; // this is weird... what is the point of rreq hops?  
    }
    else 
    {
        // we are an intermediary hop
        // TODO: copy known sequence number for destination into destSeq field
        // TODO: put hop count from routing table 
    }
    rrep.destSeqNum = rreq.destSeqNum;
    rrep.origIP = rreq.origIP;
    rrep.lifetime = MY_ROUTE_TIMEOUT_MS;

    // TODO: update routing table! 

    return rrep;
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
