#include "aodv.h"
#include "string.h"
#include "send_packet.h"

#define AODV_DEBUG		1

AODV::AODV(IP_ADDR ip)
{
//	cout << "Created new aodv routing protocol." << endl;

	this->ipAddress = ip;
	this->table = new AODVRoutingTable();

	this->rreqHelper.setRoutingTable((AODVRoutingTable*)(this->table));
	this->rreqHelper.setIp(ip);
}

AODV::~AODV()
{
	delete this->table;
}

void AODV::decodeReceivedPacketBuffer(char* buffer, int length, IP_ADDR source)
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
			handleRREQ(buffer, length, source);
			break;
		case 2: 
			handleRREPBuffer(buffer, length);
			break;
		case 3:
			handleRERRBuffer(buffer, length);
			break;
		default:
			if (AODV_DEBUG)
				cout << "ERROR: Packet not AODV." << endl;
			break;
	}
}

void AODV::handleRREQ(char* buffer, int length, IP_ADDR source)
{
	// handle a received rreq message

	// 1. make sure this is a valid rreq message 
	if (length != sizeof(rreqPacket))
	{
		if (AODV_DEBUG)
			cout << "ERROR handling rreq packet. Invalid length." << endl;

		return;
	}

	// valid rreq packet, make a decision
	rreqPacket rreq = rreqHelper.readRREQBuffer(buffer);

	// 2. is this a duplicate rreq? 
	if (rreqHelper.isDuplicateRREQ(rreq))
	{
		if (AODV_DEBUG)
			cout << "Duplicate RREQ message received." << endl;

		return;
	}

	// 3. should we generate a rrep? are we the final destination?
	if (rreqHelper.shouldGenerateRREP(rreq))
	{
		// generate a rreq message from this rreq
		// TODO: Implement this 
		cout << "Generating RREP message..." << endl;
		// TODO: SEND PACKET
		return;
	}

	// 4. not final destination, forward the rreq 
	rreqPacket forwardRREQ = rreqHelper.createForwardRREQ(rreq, source);
	// TODO: SEND PACKET 
	sendBuffer(rreqHelper.createRREQBuffer(forwardRREQ), sizeof(forwardRREQ), this->getIp(), getIpFromString("255.255.255.255"));
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
