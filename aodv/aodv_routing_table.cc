#include "aodv_routing_table.h"

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

uint8_t AODVRoutingTable::getDestHopCount(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->table.count(dest))
	{
		// entry exists, return dest hop count  
		return ((AODVInfo*)&(this->table[dest]))->hopCount; 
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

void AODVRoutingTable::setHopCount(const IP_ADDR dest, uint8_t hopCount)
{
	// check if this entry exists 
	if (this->table.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->table[dest]))->hopCount = hopCount; 
	}
	else
	{
		cout << "DEBUG: Error update hop count: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREQ(rreqPacket* receivedRREQ, IP_ADDR sourceIP)
{
	this->updateTableEntry(receivedRREQ->origIP, sourceIP);
	if (receivedRREQ->origSeqNum > getDestSequenceNumber(receivedRREQ->origIP))
	{
		this->setDestSequenceNumber(receivedRREQ->origIP, receivedRREQ->origSeqNum);
		this->setHopCount(receivedRREQ->origIP, receivedRREQ->hopCount);
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREP(rrepPacket* receivedRREP, IP_ADDR sourceIP)
{
	// found a route to the destination! Add the entry.
	// next hop is who just sent this RREP
	this->updateTableEntry(receivedRREP->destIP, sourceIP);
	if (receivedRREP->destSeqNum > getDestSequenceNumber(receivedRREP->destIP))
	{
		this->setDestSequenceNumber(receivedRREP->destIP, receivedRREP->destSeqNum);
		this->setHopCount(receivedRREP->destIP, receivedRREP->hopCount);
	}
}
