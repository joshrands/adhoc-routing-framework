#ifndef AODVRREQ_H
#define AODVRREQ_H

/*********************************
 * aodv_rreq.h
 *
 * Custom c++ implementation of the aodv rreq message 
 * 
 * Author: Josh Rands
 * Date: 9/18/2019
 ********************************/

#include "aodv_defines.h"
#include "aodv_packets.h"
#include "aodv_routing_table.h"

class RREQHelper 
{
public: 
	// default constructor
	RREQHelper();
	RREQHelper(IP_ADDR ip, AODVRoutingTable* table, uint32_t* seqNum);

	void setIp(const IP_ADDR ip) { this->m_ip = ip; }
	void setRoutingTable(AODVRoutingTable* table) { this->m_pTable = table; }
	void setSequenceNumPointer(uint32_t* seqNum) { this->m_pSequenceNum = seqNum; }

	// check if a rrep should be generated
	bool shouldGenerateRREP(rreqPacket receivedRREQ);

	// initiating RREQ enters state of waiting for RREP
	rreqPacket createRREQ(const IP_ADDR destIP, const uint32_t destSeqNum=0);
	// forward RREQ enters state of maybe receiving RREP
	rreqPacket createForwardRREQ(rreqPacket receivedRREQ, IP_ADDR sourceIP);
	// convert rreq message to a char* buffer
	static char* createRREQBuffer(const rreqPacket rreq);
	// read a received rreq buffer
	static rreqPacket readRREQBuffer(char* buffer);
	// return true if this is a duplicate rreq
	bool isDuplicateRREQ(rreqPacket receivedRREQ);

private:
	AODVRoutingTable* m_pTable;
	IP_ADDR m_ip;
	uint32_t m_rreqID;
	uint32_t* m_pSequenceNum;	
};

#endif