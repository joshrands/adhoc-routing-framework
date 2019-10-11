#include "aodv_routing_table.h"

AODVRoutingTable::AODVRoutingTable()
{
	if (TABLE_DEBUG)
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
	if (TABLE_DEBUG)
		cout << "Updating routing table from RREQ packet" << endl;

	if (receivedRREQ->hopCount == 0)
		cout << "HOP COUNT IS ZERO. FROM " << getStringFromIp(sourceIP) << endl;

	if (  receivedRREQ->origSeqNum > getDestSequenceNumber(receivedRREQ->origIP)
	   || getCostOfDest(receivedRREQ->origIP) > getCostOfRREQ(*receivedRREQ))
	{
		this->updateTableEntry(receivedRREQ->origIP, sourceIP);

		this->setDestSequenceNumber(receivedRREQ->origIP, receivedRREQ->origSeqNum);
		this->setHopCount(receivedRREQ->origIP, receivedRREQ->hopCount);
		this->setLastRREQId(receivedRREQ->origIP, receivedRREQ->rreqID);
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREP(rrepPacket* receivedRREP, IP_ADDR sourceIP)
{
	if (TABLE_DEBUG)
		cout << "Updating routing table from RREP packet" << endl;

	if (  receivedRREP->destSeqNum > getDestSequenceNumber(receivedRREP->destIP)
	   || getCostOfDest(receivedRREP->destIP) > getCostOfRREP(*receivedRREP))
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
		if (TABLE_DEBUG)
			cout << "Updating existing AODV entry" << endl;
		this->m_aodvTable[dest].nextHop = nextHop;
		this->m_aodvTable[dest].ttl = DEFAULT_TTL;
	}
	else
	{
		// no entry, create new 
		if (TABLE_DEBUG)
			cout << "Creating new AODV entry" << endl;
		AODVInfo info;
		info.dest = dest;
		info.nextHop = nextHop;
		info.ttl = DEFAULT_TTL;
	
		this->m_aodvTable[dest] = info;
	}

	this->m_aodvTable[dest].active = true;
}

void AODVRoutingTable::removeTableEntry(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{	
		// entry exists, delete entry 
		if (TABLE_DEBUG)
			cout << "Setting table entry as inactive" << endl;

		this->m_aodvTable[dest].active = false;
	}
	else
	{
		// no entry, create new 
		if (TABLE_DEBUG)
			cout << "Error. Tried to erase non-existent table entry." << endl;
	}
}

int AODVRoutingTable::getCostOfDest(const IP_ADDR dest)
{
	// currently using hopcount...
	return getDestHopCount(dest);
}

int AODVRoutingTable::getCostOfRREQ(const rreqPacket rreq)
{
	// currently using hopcount...
	return rreq.hopCount;
}

int AODVRoutingTable::getCostOfRREP(const rrepPacket rrep)
{
	// currently using hopcount...
	return rrep.hopCount;
}

bool AODVRoutingTable::getIsRouteActive(const IP_ADDR dest)
{	
	if (m_aodvTable.count(dest) > 0)
		return m_aodvTable[dest].active;
	else 
		return false;
}
