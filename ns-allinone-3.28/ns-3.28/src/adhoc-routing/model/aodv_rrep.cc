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
    // update the routing table from the original rreq message
    // update hop count. this is correct!!!
    rreq.hopCount++;
    this->m_pTable->updateAODVRoutingTableFromRREQ(&(rreq), source);

	rrepPacket rrep;    

    // populate fields of rrep
    rrep.type = 0x02;
    rrep.destIP = rreq.destIP;
    // is this node the destination? 
    if (rrep.destIP == this->m_ip)
    {
        if (RREP_DEBUG)
            cout << "[RREP]:[DEBUG]: Creating RREP from final destination" << endl;
        // yes, copy this sequence number in
        // increment this node sequence number if equal to orig sequence number 
        if (*(this->m_pSequenceNum) == rreq.destSeqNum)
				{
        		*(this->m_pSequenceNum) = *(this->m_pSequenceNum) + 1;
				}

        rrep.destSeqNum = *(this->m_pSequenceNum);
        rrep.hopCount = 0; // this is weird... what is the point of rrep hops?  
    }
    else 
    {
        if (RREP_DEBUG)
            cout << "[RREP]:[DEBUG]: Generating RREP from intermediary hop" << endl;
        // we are an intermediary hop
        // copy known sequence number for destination into destSeq field
        rrep.destSeqNum = this->m_pTable->getDestSequenceNumber(rreq.destIP);
        // put hop count from routing table 
        rrep.hopCount = this->m_pTable->getDestHopCount(rreq.destIP);
    }

    rrep.origIP = rreq.origIP;
    rrep.lifetime = MY_ROUTE_TIMEOUT_MS;

    return rrep;
}

rrepPacket RREPHelper::createForwardRREP(rrepPacket receivedRREP, IP_ADDR source)
{
    // TODO: Longest prefix matching? We are in the same subnet...
    rrepPacket forwardRREP = receivedRREP;
    forwardRREP.hopCount++;

    // update routing table for destination IF
    // 1. TODO: Sequence number in routing table is invalid?
    // 2. The destination sequence number is greater than the stored one
    // 3. TODO: Route is inactive? 
    // 4. Sequence numbers are the same, but the new hop count is smaller
    if  (receivedRREP.destSeqNum > this->m_pTable->getDestSequenceNumber(receivedRREP.destIP)
     || (receivedRREP.hopCount < this->m_pTable->getDestHopCount(receivedRREP.destIP)))
    {
        this->m_pTable->updateAODVRoutingTableFromRREP(&forwardRREP, source);
    }


    return forwardRREP;
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
