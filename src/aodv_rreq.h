/*********************************
 * aodv_rreq.h
 *
 * Custom c++ implementation of the aodv rreq message 
 * 
 * Author: Josh Rands
 * Date: 9/18/2019
 ********************************/

#include "defines.h"
#include "aodv_packets.h"
#include "aodv_routing_table.h"

class RREQHelper 
{
public: 
	// default constructor
	RREQHelper();
	RREQHelper(IP_ADDR ip, AODVRoutingTable* table);

	void setIp(const IP_ADDR ip) { this->m_ip = ip; }
	void setRoutingTable(AODVRoutingTable* table) { this->m_table = table; }

	// check if a rrep should be generated
	bool shouldGenerateRREP(rreqPacket receivedRREQ);

	// handle a received rreq message 
	void handleRREQBuffer(char* buffer, int length);
	// initiating RREQ enters state of waiting for RREP
	rreqPacket createRREQ(const IP_ADDR destIP, const uint32_t destSeqNum=0);
	// forward RREQ enters state of maybe receiving RREP
	rreqPacket createForwardRREQ(rreqPacket receivedRREQ, IP_ADDR sourceIP);
	// convert rreq message to a char* buffer
	char* createRREQBuffer(const rreqPacket rreq);
	// read a received rreq buffer
	rreqPacket readRREQBuffer(char* buffer);
	// return true if this is a duplicate rreq
	bool isDuplicateRREQ(rreqPacket receivedRREQ);

private:
	AODVRoutingTable* m_table;
	IP_ADDR m_ip;
	uint32_t m_rreqID;
	uint32_t m_sequenceNum;	
};
