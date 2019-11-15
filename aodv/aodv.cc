#include "aodv.h"
#include "string.h"
#include <chrono>
#include <fstream>
#include <stdio.h>
#include <thread>

int AODV::AODV_PORT = 5432;

AODV::AODV() {
    if (AODV_DEBUG)
        cout << "[DEBUG]: Warning: Must update aodv ip address.";
}

AODV::AODV(const char* ip) : AODV(getIpFromString(ip))
{
	if (AODV_DEBUG)
		cout << "[DEBUG]: Created new aodv routing protocol." << endl;
}

AODV::AODV(IP_ADDR ip) {
    if (AODV_DEBUG)
        cout << "[DEBUG]: Created new aodv routing protocol." << endl;

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

AODV::~AODV() {
    delete this->m_aodvTable;
    m_aodvTable = NULL;

    for (map<IP_ADDR, queue<pair<char *, int>>>::iterator it =
             rreqPacketBuffer.begin();
         it != rreqPacketBuffer.end(); ++it) {
        // remove all packets from this queue
        queue<pair<char *, int>> packetQueue = it->second;
        while (packetQueue.size() > 0) {
            pair<char *, int> package = packetQueue.front();
            delete package.first;

            packetQueue.pop();
        }
        this->rreqPacketBuffer.erase(it->first);
    }
}

void AODV::receivePacket(char *packet, int length, IP_ADDR source) {
    // get final destination
    IP_ADDR finalDestination;
    memcpy(&finalDestination, &(packet[1]), 4);
    IP_ADDR origIP;
    memcpy(&origIP, &(packet[5]), 4);

    if (this->getIp() == finalDestination) {
        // packet has reached its final destination!
        receivedPackets.emplace_back(pair<char *, int>(packet, length));
        if (AODV_PRINT_PACKET) {
            cout << "[PACKET]: Node " << getStringFromIp(this->getIp())
                 << " received packet: " << endl;

            for (int i = HEADER_SIZE; i < length; i++) {
                cout << packet[i];
            }
            cout << endl;
        }
        // output the contents of this packet
        if (AODV_LOG_OUTPUT) {
            ofstream logFile;
            logFile.open("./logs/" + getStringFromIp(this->getIp()) +
                             "-packet-log.txt",
                         ios::out);

            for (int i = HEADER_SIZE; i < length; i++) {
                logFile << packet[i];
            }

            logFile.close();

            while (logFile.is_open())
                ;
        }

        return;
    } else {
        // send the packet to final destination - will check routing table
        // strip header and send packet
        // TODO: Most important time to check link state.
        packet += HEADER_SIZE;
        sendPacket(packet, length - HEADER_SIZE, finalDestination, origIP);
    }

    if (AODV_LOG_OUTPUT)
        logRoutingTable();
}

void AODV::sendPacket(char *packet, int length, IP_ADDR finalDestination,
                      IP_ADDR origIP) {
    // by default this node is the originator
    if (-1 == signed(origIP))
        origIP = getIp();

    // if entry exists in routing table, send it!
    // check routing table
    IP_ADDR nextHop = this->getTable()->getNextHop(finalDestination);

    if (false == getTable()->getIsRouteActive(finalDestination)) {
        char *bufferedPacket = (char *)(malloc(sizeof(char) * length));
        memcpy(bufferedPacket, packet, length);
        // Put this packet in a buffer to be sent when a route opens up
        if (this->rreqPacketBuffer.count(finalDestination)) {
            this->rreqPacketBuffer[finalDestination].push(
                pair<char *, int>(bufferedPacket, length));
        } else {
            this->rreqPacketBuffer[finalDestination] = queue<pair<char *, int>>(
                {pair<char *, int>(bufferedPacket, length)});
        }

        if (nextHop != 0) {
            // this route is temporarily unavailable... check if actively
            // finding a route
            // TODO: send a RERR

            return;
        }

        if (AODV_DEBUG)
            cout << "[DEBUG]: No existing route, creating RREQ message."
                 << endl;

        rreqPacket rreq = this->rreqHelper.createRREQ(finalDestination);

        // start a thread for THIS rreq and wait for a response
        if (RREQ_RETRIES) {
            thread waitForResponse(retryRouteRequestIfNoRREP, this, rreq,
                                   RREQ_RETRIES);
            waitForResponse.detach();
        }
        broadcastRREQBuffer(rreq);

        return;
    }

    if (AODV_DEBUG)
        cout << "[DEBUG]: Route exists. Routing from "
             << getStringFromIp(getIp()) << " to "
             << getStringFromIp(finalDestination) << endl;

    // add aodv header to buffer
    char *buffer = (char *)(malloc(HEADER_SIZE + length));
    uint8_t zero = 0x00;
    memcpy(buffer, &(zero), 1);
    buffer++;
    memcpy(buffer, &finalDestination, 4);
    buffer += 4;
    memcpy(buffer, &origIP, 4);
    buffer += 4;

    // copy data into packet
    memcpy(buffer, packet, length);
    // reset packet
    buffer -= HEADER_SIZE;

    if (linkExists(nextHop)) {
        socketSendPacket(buffer, length + HEADER_SIZE, nextHop, DATA_PORT);
    } else {
        repairLink(nextHop, finalDestination, buffer, length, origIP,
                   DATA_PORT);
    }

    delete buffer;
}

void AODV::broadcastRREQBuffer(rreqPacket rreq) {
    if (AODV_DEBUG)
        cout << "[DEBUG]: Broadcasting Route Request from node "
             << getStringFromIp(getIp()) << endl;

    char *rreqBuffer = RREQHelper::createRREQBuffer(rreq);
    socketSendPacket(rreqBuffer, sizeof(rreq), getIpFromString(BROADCAST),
                     AODV_PORT);

    delete rreqBuffer;
}

void AODV::decodeReceivedPacketBuffer(char *buffer, int length,
                                      IP_ADDR source, int port) {
    //	cout << "Node " << getStringFromIp(getIp()) << " received a packet. " <<
    // endl;

    if (length <= 0) {
        cout << "[ERROR]: ERROR DECODING PACKET. Length = 0." << endl;
    }

    // determine type of message
    uint8_t type;
    memcpy(&type, buffer, 1);

    switch (type) {
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

void AODV::handleRREQ(char *buffer, int length, IP_ADDR source) {
    // handle a received rreq message

    // 1. make sure this is a valid rreq message
    if (length != sizeof(rreqPacket)) {
        if (AODV_DEBUG)
            cout << "[ERROR]: Error handling rreq packet. Invalid length of " << length
                 << " should be " << sizeof(rreqPacket) << endl;

        return;
    }

    // valid rreq packet, make a decision
    rreqPacket rreq = rreqHelper.readRREQBuffer(buffer);

    // 2. is this a duplicate rreq?
    if (rreqHelper.isDuplicateRREQ(rreq)) {
        if (RREQ_DEBUG)
            cout << "[DEBUG]: Duplicate RREQ message received." << endl;

        return;
    }

    // 3. should we generate a rrep? are we the final destination?
    if (rreqHelper.shouldGenerateRREP(rreq)) {
        // generate a rreq message from this rreq
        // TODO: Implement this
        if (RREP_DEBUG)
            cout << "[DEBUG]: Generating RREP message..." << endl;

        rrepPacket rrep = rrepHelper.createRREPFromRREQ(rreq, source);

        // convert packet to buffer and send
        char *buffer;
        buffer = RREPHelper::createRREPBuffer(rrep);

        IP_ADDR nextHopIp = getTable()->getNextHop(rrep.origIP);

        if (AODV_DEBUG)
            cout << "[DEBUG]: Next hop for rrep: " << getStringFromIp(nextHopIp)
                 << " from " << getStringFromIp(this->getIp()) << endl;

        if (linkExists(nextHopIp))
            socketSendPacket(buffer, sizeof(rrep), nextHopIp, AODV_PORT);
        //		else
        //			repairLink(nextHopIp, rrep.origIP, buffer, sizeof(rrep),
        // getIp(), AODV_PORT); 		we don't try to repair links for
        // rreps...

        delete buffer;

        return;
    }

    // 4. not final destination, forward the rreq
    rreqPacket forwardRREQ = rreqHelper.createForwardRREQ(rreq, source);
    broadcastRREQBuffer(forwardRREQ);
}

void AODV::handleRREP(char *buffer, int length, IP_ADDR source) {
    // handle a received rrep message

    // 1. make sure this is a valid rrep message
    if (length != sizeof(rrepPacket)) {
        if (AODV_DEBUG)
            cout << "[DEBUG]: ERROR handling rrep packet. Invalid length."
                 << endl;

        return;
    }

    // valid rreq packet, make a decision
    rrepPacket rrep = rrepHelper.readRREPBuffer(buffer);

    this->getTable()->updateAODVRoutingTableFromRREP(&rrep, source);

    // 2. are we the original ip? was this our route request reply?
    if (this->getIp() == rrep.origIP) {
        // packet made it back!
        if (AODV_DEBUG)
            cout << "[DEBUG]: Route discovery complete! Sending buffered "
                    "packets, if they exist."
                 << endl;

        // Send any queued packages for that destination
        if (this->rreqPacketBuffer.count(rrep.destIP)) {
            if (AODV_DEBUG)
                cout << "[DEBUG]: There are packets in the queue. Sending "
                        "data..."
                     << endl;

            // Pull all the packets off the queue and send them
            queue<pair<char *, int>> *packetQueue =
                &rreqPacketBuffer[rrep.destIP];
            while (packetQueue->size() > 0) {
                pair<char *, int> package = packetQueue->front();
                sendPacket(package.first, package.second, rrep.destIP,
                           DATA_PORT);
                delete package.first;

                packetQueue->pop();
            }
        }
        this->rreqPacketBuffer.erase(rrep.destIP);

    } else {
        // forward this packet
        rrepPacket forwardRREP =
            this->rrepHelper.createForwardRREP(rrep, source);

        IP_ADDR nextHopIp = this->getTable()->getNextHop(forwardRREP.origIP);

        if (RREP_DEBUG)
            cout << "[DEBUG]: Forward rrep from " << getStringFromIp(getIp())
                 << " to " << getStringFromIp(nextHopIp) << endl;

        char *buffer = RREPHelper::createRREPBuffer(forwardRREP);

        if (linkExists(nextHopIp))
            socketSendPacket(buffer, sizeof(forwardRREP), nextHopIp, AODV_PORT);
        else if (RREP_DEBUG)
            cout << "[DEBUG]: We don't try to repair failed RREPs..." << endl;
        //			repairLink(nextHopIp, forwardRREP.origIP, buffer,
        // sizeof(forwardRREP), forwardRREP.destIP, AODV_PORT);

        delete buffer;
    }
}

void AODV::handleRERR(char *buffer, int length, IP_ADDR source) {
    // handle a received rerr message. most likely forwarding it...
    if (RERR_DEBUG)
        cout << "[DEBUG]: Handling RERR message..." << endl;

    rerrPacket rerr = RERRHelper::readRERRBuffer(buffer);

    // when a link breaks, mark a routing table entry as invalid
    if (RERR_DEBUG)
        cout << "[DEBUG]: Setting unreachable IP "
             << getStringFromIp(rerr.unreachableIP) << " to INACTIVE." << endl;

    getTable()->setIsRouteActive(rerr.unreachableIP, false);

    char *packet = (char *)(malloc(sizeof(rerr)));
    memcpy(packet, &rerr, sizeof(rerr));

    // when forwarding packets, make sure the routing table entry is valid
    if (rerr.origIP == this->getIp()) {
        if (RERR_DEBUG)
            cout << "[DEBUG]: Route Error response received by sender. "
                    "Generating a new RREQ..."
                 << endl;

        // generate a RREQ to the destination
        rreqPacket rreq = rreqHelper.createRREQ(rerr.unreachableIP);
        broadcastRREQBuffer(rreq);
    } else if (getTable()->getIsRouteActive(rerr.origIP)) {
        // send a packet to the next hop about the error
        IP_ADDR nextHop = getTable()->getNextHop(rerr.origIP);
        socketSendPacket(packet, sizeof(rerr), nextHop, AODV_PORT);
    }
}

void AODV::repairLink(IP_ADDR brokenLink, IP_ADDR finalDest, char *buffer,
                      int length, IP_ADDR origIP, int port) {
    // first try to fix the link locally
    if (true == attemptLocalRepair(brokenLink, finalDest)) {
        if (MONITOR_DEBUG)
            cout << "[DEBUG]: Broken link repaired locally! Sending packet..."
                 << endl;

        IP_ADDR nextHop = getTable()->getNextHop(finalDest);
        socketSendPacket(buffer, length, nextHop, port);
    } else {
        // link is totally dead. Remove entry in routing table and send a RERR.
        rerrPacket rerr = rerrHelper.createRERR(finalDest, origIP);
        char *packet = (char *)(malloc(sizeof(rerr)));
        memcpy(packet, &rerr, sizeof(rerr));

        // remove from routing table
        getTable()->setIsRouteActive(finalDest, false);

        // send reverse rerr to originator
        IP_ADDR nextHop = getTable()->getNextHop(origIP);
        cout << "Next hop for rerr = " << getStringFromIp(nextHop) << endl;

        // TODO: ADD PACKET TO PACKET BUFFER QUEUE
        socketSendPacket(packet, sizeof(rerr), nextHop, AODV_PORT);
    }
}

bool AODV::linkExists(IP_ADDR dest) {
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Checking if link exists from "
             << getStringFromIp(getIp()) << " to " << getStringFromIp(dest)
             << endl;

    // update current list of one hop neighbors
    getOneHopNeighbors();

    for (IP_ADDR ip : m_neighbors) {
        if (dest == ip) {
            if (MONITOR_DEBUG)
                cout << "[DEBUG]: Link exists!" << endl;

            return true;
        }
    }

    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Link does not exist." << endl;

    return false;
}

bool AODV::attemptLocalRepair(IP_ADDR brokenLink, IP_ADDR finalDest) {
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Attempting local repair from broken link "
             << getStringFromIp(brokenLink) << " to destination "
             << getStringFromIp(finalDest) << endl;

    // TODO: Use network monitoring to attempt local repair
    return false;
}

void AODV::getOneHopNeighbors() {}

void AODV::logRoutingTable() {
    ofstream logFile;
    logFile.open("./logs/" + getStringFromIp(this->getIp()) + "-rtable.txt",
                 ios::out);

    if (logFile.is_open())
        logFile << "AODV Log for node " << this->getIp() << endl;

    // check if there are any entries
    if (this->getTable()->getInternalAODVTable().size() == 0) {
        logFile << "Routing table empty." << endl;
        return;
    }

    map<IP_ADDR, AODVInfo>::iterator it;

    logFile << "DESTINATION IP : NEXT HOP : TOTAL HOPS : STATUS" << endl;

    for (it = this->getTable()->getInternalAODVTable().begin();
         it != this->getTable()->getInternalAODVTable().end(); it++) {
        logFile << getStringFromIp(it->first) << " : "
                << getStringFromIp(it->second.nextHop) << " : "
                << to_string(it->second.hopCount);

        if (getTable()->getIsRouteActive(it->first))
            logFile << " : ACTIVE" << endl;
        else
            logFile << " : INACTIVE" << endl;
    }

    logFile.close();

    while (logFile.is_open())
        ;
}

int AODVTest::globalPacketCount = 0;
IP_ADDR AODVTest::lastNode = 0;
IP_ADDR AODVTest::lastReceive = 0;

int AODVTest::socketSendPacket(char *buffer, int length, IP_ADDR dest,
                               int port) {
    for (uint32_t i = 0; i < m_physicalNeighbors.size(); i++) {
        if (dest == m_physicalNeighbors.at(i)->getIp() ||
            dest == getIpFromString(BROADCAST)) {
            // send packet to this node
            AODVTest::lastReceive = m_physicalNeighbors.at(i)->getIp();
            AODVTest::lastNode = getIp();
            AODVTest::globalPacketCount++;

            m_physicalNeighbors.at(i)->decodeReceivedPacketBuffer(
                buffer, length, getIp(), port);
        }
    }

    return 0;
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

bool AODVTest::isNeighbor(AODVTest node) {
    for (uint32_t i = 0; i < m_physicalNeighbors.size(); i++) {
        if (node.getIp() == m_physicalNeighbors.at(i)->getIp())
            return true;
    }

    return false;
}

bool AODVTest::packetInRreqBuffer(IP_ADDR dest) {
    return (rreqPacketBuffer.count(dest));
}

void retryRouteRequestIfNoRREP(AODV *aodv, rreqPacket sendRREQ,
                               int numberOfRetriesRemaining) {
    if (RREQ_DEBUG)
        cout << "[DEBUG]: Waiting for RREP..." << endl;

    // wait net traversal time
    this_thread::sleep_for(chrono::milliseconds(NET_TRAVERSAL_TIME_MS * 2));

    if (RREQ_DEBUG)
        cout << "[DEBUG]: Checking if RREP has been received for RREQ" << endl;

    // check if rreq with same sequence id is contained in the packet buffer
    if (numberOfRetriesRemaining - 1 > 0 &&
        aodv->getTable()->getNextHop(sendRREQ.destIP) != 0) {
        // RREQ not received... try again
        retryRouteRequestIfNoRREP(aodv, sendRREQ, numberOfRetriesRemaining - 1);
    } else {
        // RREQ received! exit thread
        if (RREQ_DEBUG)
            cout << "[DEBUG]: No longer retrying RREQ." << endl;

        exit(0);
    }
}
