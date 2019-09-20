#pragma once 

/*********************************
 * aodv_routing_table.h
 *
 * Custom c++ implementation of the aodv routing table  
 * 
 * Author: Josh Rands
 * Date: 9/18/2019
 ********************************/

// include for routing table 
// TODO: Pull routing table out of RoutingProtocol.h
#include "RoutingProtocol.h"
#include "aodv_packets.h"


class AODVInfo : public TableInfo
{
public:
	uint32_t destSequenceNumber;
	uint8_t hopCount;
};


class AODVRoutingTable : public RoutingTable
{
public:
	void updateAODVRoutingTableFromRREQ(rreqPacket* receivedRREQ, IP_ADDR sourceIP);

	uint32_t getDestSequenceNumber(const IP_ADDR dest);
	void setDestSequenceNumber(const IP_ADDR dest, uint32_t destSeqNum);
	void setHopCount(const IP_ADDR dest, uint8_t hopCount);
};
