#include "RoutingProtocol.h"

#define ROUTE_DEBUG		0

#include <fstream>

using namespace std;

IP_ADDR getIpFromString(string ipStr)
{
	IP_ADDR ip;

	struct sockaddr_in sa;
	char str[INET_ADDRSTRLEN];
	inet_pton(AF_INET, ipStr.c_str(), &(sa.sin_addr));

	ip = sa.sin_addr.s_addr;
/*
	istringstream iss(ipStr);
	string num;

	int pow = 24;
	while (getline(iss, num, '.'))
	{
		// increment ip  
		ip += (stoi(num) << pow);
		pow -= 8;
	}

	//cout << ipStr << " = " << ip << endl;
*/
	return ip;
}

string getStringFromIp(IP_ADDR ip)
{
/*	string ipString = "";
	
	ipString += to_string((uint8_t)(ip>>24)) + ".";
	ipString += to_string((uint8_t)(ip>>16)) + ".";
	ipString += to_string((uint8_t)(ip>>8)) + ".";
	ipString += to_string((uint8_t)ip);
*/
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ip), str, INET_ADDRSTRLEN);

	string ipString(str);

	return ipString;
}

RoutingTable::RoutingTable()
{
	if (ROUTE_DEBUG)	
		cout << "Routing table created" << endl;
}

RoutingTable::~RoutingTable()
{
/*	map<IP_ADDR, TableInfo*>::iterator it;

	for ( it = this->table.begin(); it != this->table.end(); it++ )
	{
		cout << "Found allocated memory" << endl;
		delete (it->second);
	}
*/
	this->table.empty();
}

IP_ADDR RoutingTable::getNextHop(const IP_ADDR dest)
{
	IP_ADDR nextHop;

	if (this->table.count(dest))
	{
//		cout << "Entry exists." << endl;
		nextHop = table[dest].nextHop;
	}
	else
	{
//		cout << "Table entry does not exist." << endl;
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
		cout << "Creating new entry" << endl;
		TableInfo info;
		info.dest = dest;
		info.nextHop = nextHop;
		info.ttl = DEFAULT_TTL;
	
		this->table[dest] = info;
	}
}

RoutingProtocol::RoutingProtocol()
{

}


