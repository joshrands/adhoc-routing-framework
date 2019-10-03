#include "aodv_routing_table.h"

AODVRoutingTable::AODVRoutingTable()
{
	cout << "Created new aodv routing table." << endl;
}

AODVRoutingTable::~AODVRoutingTable()
{

}

IP_ADDR AODVRoutingTable::getNextHop(const IP_ADDR dest)
{
	IP_ADDR nextHop;

	if (this->m_aodvTable.count(dest))
	{
		nextHop = m_aodvTable[dest].nextHop;
	}
	else
	{
		nextHop = 0;
	}

	return nextHop;
}

uint32_t AODVRoutingTable::getDestSequenceNumber(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, return dest sequence number  
		return ((AODVInfo*)&(this->m_aodvTable[dest]))->destSequenceNumber; 
	}
	else
	{
		return 0;
	}
}

uint8_t AODVRoutingTable::getDestHopCount(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, return dest hop count  
		return ((AODVInfo*)&(this->m_aodvTable[dest]))->hopCount; 
	}
	else
	{
		return 0;
	}
}

uint32_t AODVRoutingTable::getLastRREQId(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, return dest sequence number  
		return ((AODVInfo*)&(this->m_aodvTable[dest]))->lastRREQId; 
	}
	else
	{
		return 0;
	}
}

void AODVRoutingTable::setDestSequenceNumber(const IP_ADDR dest, uint32_t destSeqNum)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->m_aodvTable[dest]))->destSequenceNumber = destSeqNum; 
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
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->m_aodvTable[dest]))->hopCount = hopCount; 
	}
	else
	{
		cout << "DEBUG: Error update hop count: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::setLastRREQId(const IP_ADDR dest, uint32_t lastRREQId)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->m_aodvTable[dest]))->lastRREQId = lastRREQId; 
	}
	else
	{
		cout << "DEBUG: Error update last rreq id: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREQ(rreqPacket* receivedRREQ, IP_ADDR sourceIP)
{
	if (receivedRREQ->origSeqNum > getDestSequenceNumber(receivedRREQ->origIP))
	{
		this->updateTableEntry(receivedRREQ->origIP, sourceIP);

		this->setDestSequenceNumber(receivedRREQ->origIP, receivedRREQ->origSeqNum);
		this->setHopCount(receivedRREQ->origIP, receivedRREQ->hopCount);
		this->setLastRREQId(receivedRREQ->origIP, receivedRREQ->rreqID);
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREP(rrepPacket* receivedRREP, IP_ADDR sourceIP)
{
	if (receivedRREP->destSeqNum > getDestSequenceNumber(receivedRREP->destIP))
	{
		this->updateTableEntry(receivedRREP->destIP, sourceIP);

		this->setDestSequenceNumber(receivedRREP->destIP, receivedRREP->destSeqNum);
		this->setHopCount(receivedRREP->destIP, receivedRREP->hopCount);
	}
}

void AODVRoutingTable::updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update existing 
		this->m_aodvTable[dest].nextHop = nextHop;
		this->m_aodvTable[dest].ttl = DEFAULT_TTL;
	}
	else
	{
		// no entry, create new 
		cout << "Creating new AODV entry" << endl;
		AODVInfo info;
		info.dest = dest;
		info.nextHop = nextHop;
		info.ttl = DEFAULT_TTL;
	
		this->m_aodvTable[dest] = info;
	}
}
