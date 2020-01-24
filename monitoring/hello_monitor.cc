#include "hello_monitor.h"
#include "monitor_defines.h"
#include <iostream>
#include <cstring>

int HelloNeighbors::HELLO_INTERVAL_MS = 1000;

HelloNeighbors::HelloNeighbors(RoutingProtocol* routingProtocol)
{
    this->m_parentIp = routingProtocol->getIp();
    this->m_routing = routingProtocol;
}

HelloNeighbors::~HelloNeighbors()
{

}

void HelloNeighbors::initializeHellos()
{
    if (nullptr == m_routing)
    {
        std::cout << "[ERROR]: HELLO routing protocol not set." << std::endl;
        return;
    }
}

void HelloNeighbors::_updateNeighbors()
{
    // 1. Update neighbors of routing protocol
    m_routing->resetLinks();
    for (IP_ADDR link : m_neighbors)
    {
        m_routing->addExistingLink(link);
    }

    // 2. Clear neighbors for next time period 
    m_neighbors.clear();

    // 3. Wait 1/2 hello interval
    _sleep(HELLO_INTERVAL_MS / 2);

    // 4. Broadcast hello message  
    _broadcastHelloMessage();

    // 5. Update neighbors from received hellos 
    // this is happening on a different thread :)
    // TODO: In handlePacket for this Port??? @Zach 

    // 6. Wait 1/2 hello interval
    _sleep(HELLO_INTERVAL_MS / 2);

    // 7. Repeat
    _updateNeighbors();
}

void HelloNeighbors::_broadcastHelloMessage()
{
    char* buffer = (char*)(malloc(4));
    memcpy(buffer, &m_parentIp, 4);

    m_routing->sendPacket(buffer, 4, getIpFromString(BROADCAST_STR), m_parentIp);
}

void HelloNeighbors::_receiveHelloMessage(IP_ADDR nodeIp)
{
    // if this neighbor doesn't already exist, add it to the vector 
    if (m_neighbors.count(nodeIp) == 0)
    {
        m_neighbors.insert(nodeIp);
    }
}

bool HelloTest::_sleep(int DURATION_MS)
{
    if (HELLO_DEBUG)
        std::cout << "[DEBUG]: Sleeping for " << DURATION_MS << " ms" << std::endl;
}
