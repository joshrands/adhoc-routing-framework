#include "aodv_test.h"
#include <string.h>

// AODV TEST CLASS

int AODVTest::globalPacketCount = 0;
IP_ADDR AODVTest::lastNode = 0;
IP_ADDR AODVTest::lastReceive = 0;

bool AODVTest::_socketSendPacket(int port, char *buffer, int length, IP_ADDR dest) {
    for (uint32_t i = 0; i < m_physicalNeighbors.size(); i++) {
        if ((dest == m_physicalNeighbors.at(i)->getIp() ||
             dest == getIpFromString(BROADCAST_STR)))
        {
            // send packet to this node
            AODVTest::lastReceive = m_physicalNeighbors.at(i)->getIp();
            AODVTest::lastNode = getIp();
            AODVTest::globalPacketCount++;

			// Create QueuedPacket
			QueuedPacket p = {buffer, length, port, this->ipAddress};
			// Add to neighbors queue
            m_physicalNeighbors.at(i)->packetQueue.push(p);
			// Have it handle that packet
			m_physicalNeighbors.at(i)->handlePackets();
        }
        else 
        {	
			// TODO: implement this part
            if (DEBUG)
                cout << "[DEBUG]: " << getStringFromIp(dest) << " from " << getStringFromIp(this->ipAddress) << endl;
            if (AODV_DEBUG)
                cout << "[DEBUG]: Testing sending monitoring packet..." << endl;
        }
    }

    return true;
}

void AODVTest::addNeighbor(AODVTest *node) {
    this->m_neighbors.push_back(node->getIp());
    this->m_physicalNeighbors.push_back(node);
}

void AODVTest::removeNeighbor(AODVTest *node) {
    for (uint32_t i = 0; i < m_physicalNeighbors.size(); i++) {
        if (node->getIp() == m_physicalNeighbors.at(i)->getIp()) {
            m_neighbors.erase(m_neighbors.begin() + i);
            m_physicalNeighbors.erase(m_physicalNeighbors.begin() + i);
        }
    }
}

void AODVTest::addPhysicalNeighborOnly(AODVTest* node)
{
    this->m_physicalNeighbors.push_back(node);
}

bool AODVTest::isNeighbor(AODVTest node) {
    for (uint32_t i = 0; i < m_neighbors.size(); i++) {
        if (node.getIp() == m_neighbors.at(i))
            return true;
    }

    return false;
}

bool AODVTest::packetInRreqBuffer(IP_ADDR dest) {
    return (rreqPacketBuffer.count(dest));
}

int AODVTest::handlePackets(){
	int count = packetQueue.size();
	while(!packetQueue.empty()){
		QueuedPacket p = packetQueue.front();
		// Pop the packet off the queue
		packetQueue.pop();
		if(p.portId == ROUTING_PORT){
			_handleAODVPacket(p.data, p.length, p.source);
		}else{
			// Get Header section
			IP_ADDR finalDestination;
			memcpy(&finalDestination, &(p.data[1]), 4);
			if (this->getIp() == finalDestination || getStringFromIp(finalDestination) == BROADCAST_STR) {
				char* data_part = p.data+HEADER_SIZE; // Get only the data part of the packet
				ports[p.portId]->handlePacket(data_part, p.length, p.source);
			}else{
				_routePacket(p.portId, p.data, p.length);
			}
		}
	}
	return count;
}

void AODVTest::_buildPort(Port*){

}

void AODVTest::_destroyPort(Port*){
	
}
