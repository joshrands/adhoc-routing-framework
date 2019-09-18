#include "aodv.h"
#include "string.h"

AODV::AODV(IP_ADDR ip)
{
	cout << "Created new aodv routing protocol." << endl;

	this->ipAddress = ip;
	this->table = new AODVRoutingTable();

	this->rreqHelper.setRoutingTable((AODVRoutingTable*)(this->table));
	this->rreqHelper.setIp(ip);
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
			rreqHelper.handleRREQBuffer(buffer, length);
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
