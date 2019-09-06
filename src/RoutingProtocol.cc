#include "RoutingProtocol.h"

#include <iostream>

using namespace std;

RoutingTable::RoutingTable()
{
	cout << "Routing table created" << endl;
}

IP_ADDR RoutingTable::getNextHop(const IP_ADDR dest)
{
	IP_ADDR nextHop;

	if (this->table.count(dest))
	{
		cout << "Entry exists." << endl;
		nextHop = table[dest].nextHop;
	}
	else
	{
		cout << "Table entry does not exist." << endl;
		nextHop = 0;
	}

	return nextHop;
}

void RoutingTable::updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop)
{
	// check if this entry exists 
	if (this->table.count(dest))
	{
		// entry exists, update existing 
		this->table[dest].nextHop = nextHop;
		this->table[dest].ttl = DEFAULT_TTL;
	}
	else
	{
		// no entry, create new 
		TableInfo info; 
		info.dest = dest;
		info.nextHop = nextHop;
		info.ttl = DEFAULT_TTL;
	
		this->table[dest] = info;
	}
}

