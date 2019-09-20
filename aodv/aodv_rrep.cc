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

rrepPacket RREPHelper::createRREPFromRREQ(rreqPacket rreq, IP_ADDR source)
{
	rrepPacket rrep;    

    // populate fields of rrep
    rrep.type = 0x02;
    rrep.destIP = rreq.destIP;
    // is this node the destination? 
    if (rreq.destIP == this->m_ip)
    {
        // yes, copy this sequence number in
        // increment this node sequence number if equal to orig sequence number 
        if (*(this->m_pSequenceNum) == rreq.destSeqNum)
            *(this->m_pSequenceNum)++;

        rrep.destSeqNum = *(this->m_pSequenceNum);
        rrep.hopCount = 0x00; // this is weird... what is the point of rreq hops?  
    }
    else 
    {
        // we are an intermediary hop
        // copy known sequence number for destination into destSeq field
        rrep.destSeqNum = this->m_pTable->getDestSequenceNumber(rreq.destIP);
        // put hop count from routing table 
        rrep.hopCount = this->m_pTable->getDestHopCount(rreq.destIP);
    }

    rrep.origIP = rreq.origIP;
    rrep.lifetime = MY_ROUTE_TIMEOUT_MS;

    // update the routing table from the original rreq message
    this->m_pTable->updateAODVRoutingTableFromRREQ(&(rreq), source);

    return rrep;
}

rrepPacket RREPHelper::createForwardRREP(rrepPacket receivedRREP, IP_ADDR source)
{
    // TODO: Longest prefix matching? We are in the same subnet...

    receivedRREP.hopCount++;

    // update routing table for destination IF
    // 1. TODO: Sequence number in routing table is invalid?
    // 2. The destination sequence number is greater than the stored one
    // 3. TODO: Route is inactive? 
    // 4. Sequence numbers are the same, but the new hop count is smaller
    if  (receivedRREP.destSeqNum > this->m_pTable->getDestSequenceNumber(receivedRREP.destIP)
     || (receivedRREP.hopCount < this->m_pTable->getDestHopCount(receivedRREP.hopCount)))
    {
        this->m_pTable->updateAODVRoutingTableFromRREP(&receivedRREP,source);
    }

    return receivedRREP;
}

char* RREPHelper::createRREPBuffer(const rrepPacket rrep)
{
	char* buffer = (char*)(malloc(sizeof(rrep)));	

	// fill buffer with all rreq information
	memcpy(buffer, &(rrep), sizeof(rrep));

	return buffer;
}

rrepPacket RREPHelper::readRREPBuffer(char* buffer)
{
	rrepPacket rrep;

	// convert buffer to rrep 
	memcpy(&(rrep), buffer, sizeof(rrep));

	return rrep;
}
