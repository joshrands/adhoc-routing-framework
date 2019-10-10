#include "aodv.h"
#include "string.h"
#include "send_packet.h"
#include <fstream>
#include <stdio.h>

AODV::AODV()
{
	if (AODV_DEBUG)
		cout << "Warning: Must update aodv ip address.";
}

AODV::AODV(const char* ip)
{
	if (AODV_DEBUG)
		cout << "Created new aodv routing protocol." << endl;

	this->ipAddress = getIpFromString(ip);
	this->sequenceNum = 0;
	this->m_aodvTable = new AODVRoutingTable();

	this->rreqHelper.setRoutingTable(this->getTable());
	this->rreqHelper.setIp(getIp());
	this->rreqHelper.setSequenceNumPointer(&(this->sequenceNum));

	this->rrepHelper.setIp(getIp());
	this->rrepHelper.setRoutingTable(this->getTable());
	this->rrepHelper.setSequenceNum(&(this->sequenceNum));

	this->rerrHelper.setIp(getIp());
	this->rerrHelper.setRoutingTable(this->getTable());
	this->rerrHelper.setSequenceNum(&(this->sequenceNum));
}

AODV::AODV(IP_ADDR ip)
{
	if (AODV_DEBUG)
		cout << "Created new aodv routing protocol." << endl;

	this->ipAddress = ip;
	this->sequenceNum = 0;
	this->m_aodvTable = new AODVRoutingTable();

	this->rreqHelper.setRoutingTable(this->getTable());
	this->rreqHelper.setIp(ip);
	this->rreqHelper.setSequenceNumPointer(&(this->sequenceNum));

	this->rrepHelper.setIp(ip);
	this->rrepHelper.setRoutingTable(this->getTable());
	this->rrepHelper.setSequenceNum(&(this->sequenceNum));

	this->rerrHelper.setIp(ip);
	this->rerrHelper.setRoutingTable(this->getTable());
	this->rerrHelper.setSequenceNum(&(this->sequenceNum));
}

AODV::~AODV()
{
	delete this->m_aodvTable;
}

void AODV::receivePacket(char* packet, int length, IP_ADDR source)
{
	// get final destination
	IP_ADDR finalDestination;
	memcpy(&finalDestination, &(packet[1]), 4);
	
	if (this->getIp() == finalDestination)
	{
		// packet has reached its final destination! 
		// TODO: Now what? 
		if (AODV_PRINT_PACKET)
		{
			cout << "Node " << getStringFromIp(this->getIp()) << " received packet: " << endl;

			for (int i = 5; i < length; i++)
			{
				cout << packet[i];
			}			
			cout << endl;
		}
		// output the contents of this packet 
		if (AODV_LOG_OUTPUT)
		{
			ofstream logFile;
			logFile.open("./logs/" + getStringFromIp(this->getIp()) + "-packet-log.txt", ios::out);

			for (int i = 5; i < length; i++)
			{
				logFile << packet[i];
			}			

			logFile.close();

			while (logFile.is_open());
		}

		return;
	}
	else 
	{
		// send the packet to final destination - will check routing table
		// strip header and send packet
		// TODO: Most important time to check link state. 
		packet += 5;
		sendPacket(packet, length - 5, finalDestination);
	}

	if (AODV_LOG_OUTPUT)
		logRoutingTable();
}

void AODV::sendPacket(char* packet, int length, IP_ADDR finalDestination)
{
	// if entry exists in routing table, send it! 
	// check routing table 
	IP_ADDR nextHop = this->getTable()->getNextHop(finalDestination);
	if (0 == nextHop)
	{
		if (AODV_DEBUG)
			cout << "No existing route, creating RREQ message." << endl;
		// start a thread for an rreq and wait for a response 
		rreqPacket rreq = this->rreqHelper.createRREQ(finalDestination);

		broadcastRREQBuffer(rreq);
		// TODO: Put this packet in a buffer/queue to be sent when the rrep is received 
		return;
	}

	if (AODV_DEBUG)
		cout << "Route exists. Routing from " << getStringFromIp(getIp()) << " to " << getStringFromIp(finalDestination) << endl;
	// TODO: Check if there was a broken link and generate rerr 
	
	// add aodv header to buffer 
	char* buffer = (char*)(malloc(5 + length));
	uint8_t zero = 0x00;
	memcpy(buffer, &(zero), 1);
	buffer++;
	memcpy(buffer, &finalDestination, 4);
	buffer+=4;

	// copy data into packet 
	memcpy(buffer, packet, length);
	// reset packet 
	buffer-=5;

	socketSendPacket(buffer, length+5, nextHop, DATA_PORT);

	delete buffer;
}

void AODV::broadcastRREQBuffer(rreqPacket rreq)
{
	if (AODV_DEBUG)
		cout << "Broadcasting Route Request from node " << getStringFromIp(getIp()) << endl;

	char* rreqBuffer = RREQHelper::createRREQBuffer(rreq);
	socketSendPacket(rreqBuffer, sizeof(rreq), getIpFromString(BROADCAST), AODV_PORT);

	delete rreqBuffer;
}

void AODV::decodeReceivedPacketBuffer(char* buffer, int length, IP_ADDR source)
{
//	cout << "Node " << getStringFromIp(getIp()) << " received a packet. " << endl;

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
			handleRREP(buffer, length, source);
			break;
		case 3:
			handleRERR(buffer, length, source);
			break;
		default:
			// the packet is not AODV. Forward the packet 
			receivePacket(buffer, length, source);
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
		if (RREQ_DEBUG)
			cout << "Duplicate RREQ message received." << endl;

		return;
	}

	// 3. should we generate a rrep? are we the final destination?
	if (rreqHelper.shouldGenerateRREP(rreq))
	{
		// generate a rreq message from this rreq
		// TODO: Implement this 
		if (RREP_DEBUG)
			cout << "Generating RREP message..." << endl;

		rrepPacket rrep = rrepHelper.createRREPFromRREQ(rreq,source);

		// convert packet to buffer and send 
		char* buffer;
		buffer = RREPHelper::createRREPBuffer(rrep);

		IP_ADDR nextHopIp = getTable()->getNextHop(rrep.origIP);

		if (AODV_DEBUG)
			cout << "Next hop for rrep: " << getStringFromIp(nextHopIp) << " from " << getStringFromIp(this->getIp()) << endl;

		if (linkExists(nextHopIp))
			socketSendPacket(buffer, sizeof(rrep), nextHopIp, AODV_PORT);
		else 
			repairLink(nextHopIp, rrep.origIP, buffer, sizeof(rrep), nextHopIp, AODV_PORT);

		delete buffer;

		return;
	}

	// 4. not final destination, forward the rreq 
	rreqPacket forwardRREQ = rreqHelper.createForwardRREQ(rreq, source);
	broadcastRREQBuffer(forwardRREQ);
}

void AODV::handleRREP(char* buffer, int length, IP_ADDR source)
{
	// handle a received rrep message

	// 1. make sure this is a valid rrep message 
	if (length != sizeof(rrepPacket))
	{
		if (AODV_DEBUG)
			cout << "ERROR handling rrep packet. Invalid length." << endl;

		return;
	}

	// valid rreq packet, make a decision
	rrepPacket rrep = rrepHelper.readRREPBuffer(buffer);

	// 2. are with the original ip? was this our route request reply? 
    if (this->getIp() == rrep.origIP)
    {
        // packet made it back! 
        if (AODV_DEBUG)
            cout << "Route discovery complete!" << endl;

        this->getTable()->updateAODVRoutingTableFromRREP(&rrep,source);
    }
    else 
    {
        // forward this packet 
		rrepPacket forwardRREP = this->rrepHelper.createForwardRREP(rrep, source);

        IP_ADDR nextHopIp = this->getTable()->getNextHop(forwardRREP.origIP);

		if (RREP_DEBUG)
			cout << "Forward rrep from " << getStringFromIp(getIp()) << " to "<< getStringFromIp(nextHopIp) << endl;

        char* buffer = RREPHelper::createRREPBuffer(forwardRREP);

		if (linkExists(nextHopIp))
			socketSendPacket(buffer, sizeof(forwardRREP), nextHopIp, AODV_PORT);
		else
			repairLink(nextHopIp, forwardRREP.origIP, buffer, sizeof(forwardRREP), nextHopIp, AODV_PORT);		

		delete buffer;
    }
}

void AODV::handleRERR(char* buffer, int length, IP_ADDR source)
{
	// handle a received rerr message. most likely forwarding it...	
	if (RERR_DEBUG)
		cout << "Handling RERR message..." << endl;

	// when a link breaks, mark a routing table entry as invalid 
	// when forwarding packets, make sure the routing table entry is valid
}

void AODV::repairLink(IP_ADDR brokenLink, IP_ADDR finalDest, char* buffer, int length, IP_ADDR dest, int port)
{
	// first try to fix the link locally
	if (true == attemptLocalRepair(brokenLink, finalDest))
	{
		if (MONITOR_DEBUG)
			cout << "Broken link repaired locally! Sending packet..." << endl;

		socketSendPacket(buffer, length, dest, port);
	}
	else 
	{
		// link is totally dead, send a RERR.
		rerrHelper.createRERR(finalDest);
	}
}

bool AODV::linkExists(IP_ADDR dest)
{
	for (IP_ADDR ip : m_neighbors)
	{
		if (dest == ip)
			return true;
	}

	return false;
}

bool AODV::attemptLocalRepair(IP_ADDR brokenLink, IP_ADDR finalDest)
{
	if (MONITOR_DEBUG)
		cout << "Attempting local repair from broken link " << getStringFromIp(brokenLink) << " to destination " << getStringFromIp(finalDest) << endl;

	// TODO: Use network monitoring to attempt local repair
	return false;
}

void AODV::logRoutingTable()
{
	ofstream logFile;
	logFile.open("./logs/" + getStringFromIp(this->getIp()) + "-rtable.txt", ios::out);

	if (logFile.is_open())
		logFile << "AODV Log for node " << this->getIp() << endl;

	// check if there are any entries 
	if (this->getTable()->getInternalAODVTable().size() == 0)
	{
		logFile << "Routing table empty." << endl;
		return;
	}

	map<IP_ADDR, AODVInfo>::iterator it;

	logFile << "DESTINATION IP : NEXT HOP : TOTAL HOPS" << endl;

	for ( it = this->getTable()->getInternalAODVTable().begin(); it != this->getTable()->getInternalAODVTable().end(); it++ )
	{
		logFile << getStringFromIp(it->first)
				<< " : "
				<< getStringFromIp(it->second.nextHop)
				<< " : "
				<< to_string(it->second.hopCount)
				<< endl;
	}

	logFile.close();

	while (logFile.is_open());
}

int AODVTest::globalPacketCount = 0;
IP_ADDR AODVTest::lastNode = 0; 
IP_ADDR AODVTest::lastReceive = 0;

int AODVTest::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port)
{
	for (uint32_t i = 0; i < m_physicalNeighbors.size(); i++)
	{
		if (dest == m_physicalNeighbors.at(i)->getIp() || dest == getIpFromString(BROADCAST))
		{
			// send packet to this node
			AODVTest::lastReceive = m_physicalNeighbors.at(i)->getIp();
			AODVTest::lastNode = getIp();
			AODVTest::globalPacketCount++;

			m_physicalNeighbors.at(i)->decodeReceivedPacketBuffer(buffer, length, getIp());
		}
	}

	return 0;
}

void AODVTest::addNeighbor(AODVTest* node)
{
	this->m_neighbors.push_back(node->getIp());
	this->m_physicalNeighbors.push_back(node);
}

void AODVTest::removeNeighbor(AODVTest node)
{
	for (uint32_t i = 0; i < m_physicalNeighbors.size(); i++)
	{
		if (node.getIp() == m_physicalNeighbors.at(i)->getIp())
		{
			m_neighbors.erase(m_neighbors.begin() + i);
			m_physicalNeighbors.erase(m_physicalNeighbors.begin() + i);
		}
	}

}

bool AODVTest::isNeighbor(AODVTest node)
{
	for (uint32_t i = 0; i < m_physicalNeighbors.size(); i++)
	{
		if (node.getIp() == m_physicalNeighbors.at(i)->getIp())
			return true;
	}

	return false;
}
