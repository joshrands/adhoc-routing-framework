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
