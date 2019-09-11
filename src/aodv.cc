#include "aodv.h"

uint32_t AODVRoutingTable::getDestSequenceNumber(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->table.count(dest))
	{
		// entry exists, return dest sequence number  
		return ((AODVInfo*)&(this->table[dest]))->destSequenceNumber; 
	}
	else
	{
		return 0;
	}
}

void AODVRoutingTable::setDestSequenceNumber(const IP_ADDR dest, uint32_t destSeqNum)
{
	// check if this entry exists 
	if (this->table.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->table[dest]))->destSequenceNumber = destSeqNum; 
	}
	else
	{
		cout << "DEBUG: Error update sequence number: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREQ(RREQ receivedRREQ, IP_ADDR sourceIP)
{
	this->updateTableEntry(receivedRREQ.origIP, sourceIP);
	if (receivedRREQ.origSeqNum > getDestSequenceNumber(receivedRREQ.origIP))
	{
		this->setDestSequenceNumber(receivedRREQ.origIP, receivedRREQ.origSeqNum);
	}
}

AODV::AODV()
{
	cout << "Created new aodv routing protocol." << endl;

	this->table = new AODVRoutingTable();
}

AODV::~AODV()
{
	delete this->table;
}

RREQ AODV::createRREQ(const IP_ADDR dest)
{
	// Section 6.3 rfc3561

	// there is no current path to the destination, create a RREQ 
	RREQ rreq; 

	rreq.type = 0x01;
	rreq.flags = 0;
	rreq.hopCount = 0;
	
	rreq.rreqID = (++this->rreqID);
	rreq.destIP = dest;
	rreq.destSeqNum = getTable()->getDestSequenceNumber(dest);
	if (0 == rreq.destSeqNum)
	{
		// unknown destSeqNum flag is 00001000 
		rreq.flags |= 0x08;
	}
	rreq.origIP = this->getIp();
	rreq.origSeqNum = (++this->sequenceNum);

	return rreq;
}

RREQ AODV::createForwardRREQ(RREQ receivedRREQ, IP_ADDR sourceIP)
{
	// Section 6.5 rfc3561

	RREQ forwardRREQ = receivedRREQ;

	// 1. Increment the hop count 
	forwardRREQ.hopCount++;

	// 2. Update routing table for this node with orig sequence nubmer 
	this->getTable()->updateAODVRoutingTableFromRREQ(receivedRREQ, sourceIP);

	// 3. TODO: Set lifetime of table entry

	return forwardRREQ;
}

char* getRREQBuffer(const RREQ rreq)
{
	char* buffer = (char*)(malloc(sizeof(rreq)));	

	return buffer;
}

RREQ readRREQBuffer(char* buffer)
{
	RREQ rreq;

	return rreq;
}

RREP createRREP(const IP_ADDR dest)
{
/*
Immediately before a destination node originates a RREP in
response to a RREQ, it MUST update its own sequence number to the
maximum of its current sequence number and the destination
sequence number in the RREQ packet.
*/
}

void forwardRREP(const RREP receivedRREP)
{

}

char* createRREPBuffer(const RREP rrep)
{

}

RREP readRREPBuffer(char* buffer)
{

}

RERR createRERR(const IP_ADDR dest)
{

}

void forwardRERR(const RERR receivedRERR)
{

}

char* createRERRBuffer(const RERR rerr)
{

}

RERR readRERRBuffer(char* buffer)
{

}
