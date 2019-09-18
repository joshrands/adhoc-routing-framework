#include "aodv_rreq.h"
#include "defines.h"

#include <iostream>
#include <string.h>

using namespace std;

RREQHelper::RREQHelper()
{
    this->m_rreqID = 0;
    this->m_sequenceNum = 0;
}

RREQHelper::RREQHelper(IP_ADDR ip, AODVRoutingTable* table) 
{
    this->m_table = table;
    this->m_ip = ip; 
    this->m_rreqID = 0;
    this->m_sequenceNum = 0;
}

void RREQHelper::handleRREQBuffer(char* buffer, int length)
{
	cout << "Handling rreq message." << endl;
}

rreqPacket RREQHelper::createRREQ(const IP_ADDR destIP, const uint32_t destSeqNum)
{
	// Section 6.3 rfc3561

	// there is no current path to the destination, create a RREQ 
	rreqPacket rreq; 

	rreq.type = 0x01;

    // set the flags 

	rreq.hopCount = 0;
	
	rreq.rreqID = (++this->m_rreqID);
	rreq.destIP = destIP;
	rreq.destSeqNum = destSeqNum; 
	if (0 == rreq.destSeqNum)
	{
		// unknown destSeqNum flag is 00001000 
		rreq.u |= 0x08;
	}
	rreq.origIP = this->m_ip;
	rreq.origSeqNum = (++this->m_sequenceNum);

	return rreq;
}

rreqPacket RREQHelper::createForwardRREQ(rreqPacket receivedRREQ, IP_ADDR sourceIP)
{
	// Section 6.5 rfc3561

	rreqPacket forwardRREQ = receivedRREQ;

	// 1. Increment the hop count 
	forwardRREQ.hopCount++;

	// 2. Update routing table for this node with orig sequence nubmer 
	this->m_table->updateAODVRoutingTableFromRREQ(&receivedRREQ, sourceIP);

	// 3. TODO: Set lifetime of table entry

	return forwardRREQ;
}

char* RREQHelper::createRREQBuffer(const rreqPacket rreq)
{
	char* buffer = (char*)(malloc(sizeof(rreq)));	

	// fill buffer with all rreq information
	memcpy(buffer, &(rreq), sizeof(rreq));

	return buffer;
}

rreqPacket RREQHelper::readRREQBuffer(char* buffer)
{
	rreqPacket rreq;

	// convert buffer to rreq 
	memcpy(&(rreq), buffer, sizeof(rreq));

	return rreq;
}