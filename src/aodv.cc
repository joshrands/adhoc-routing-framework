#include "aodv.h"
#include "string.h"

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

void AODV::decodeReceivedPacketBuffer(char* buffer, int length)
{
	if (length <= 0)
	{
		cout << "ERROR DECODING PACKET. Length = 0." << endl;
	}

	// determine type of message 
	uint8_t type;
	memcpy(&type, buffer, 1);

	switch (type)
	{
		case 1:
			handleRREQBuffer(buffer, length);
			break;
		case 2: 
			handleRREPBuffer(buffer, length);
			break;
		case 3:
			handleRERRBuffer(buffer, length);
			break;
		default:
			cout << "Packet not AODV." << endl;
			break;
	}
}

void AODV::handleRREQBuffer(char* buffer, int length)
{
	cout << "Handling rreq message." << endl;
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

char* AODV::createRREQBuffer(const RREQ rreq)
{
	char* buffer = (char*)(malloc(sizeof(rreq)));	

	// fill buffer with all rreq information
	memcpy(buffer, &(rreq), sizeof(rreq));

	return buffer;
}

RREQ AODV::readRREQBuffer(char* buffer)
{
	RREQ rreq;

	// convert buffer to rreq 
	memcpy(&(rreq), buffer, sizeof(rreq));

	return rreq;
}

void AODV::handleRREPBuffer(char* buffer, int length)
{

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

void AODV::handleRERRBuffer(char* buffer, int length)
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
