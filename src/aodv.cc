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
	// there is no current path to the destination, create a RREQ 
	RREQ rreq; 

	// Section 6.3 rfc3561
	rreq.type = 0x01;
	rreq.flags = 0;
	rreq.hopCount = 0;
	
	rreq.rreqID = (++this->rreqID);
	rreq.destIP = dest;
	// TODO: Get the latest sequence number from routing table
	// IF UNKNOWN, SET SEQ NUM UNKOWN FLAG
	rreq.destSeqNum = ((AODVRoutingTable*)(this->table))->getDestSequenceNumber(dest);
	rreq.origIP = this->getIp();
	rreq.origSeqNum = (++this->sequenceNum);

	return rreq;
}

void forwardRREQ(const RREQ receivedRREQ)
{

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
