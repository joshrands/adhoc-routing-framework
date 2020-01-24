#include "hello_monitor.h"
#include <iostream>
#include <cstring>

int HelloNeighbors::HELLO_INTERVAL_MS = 1000;

HelloNeighbors::~HelloNeighbors()
{

}

void HelloNeighbors::handlePacket(char* data, int length, IP_ADDR source)
{
    std::cout << "[HELLO][WARNING]: Handle packets not implemented" << std::endl;

    // if a hello message was received from SOURCE, then that is a neighbor! 
    m_neighbors.insert(source);
} 

void HelloNeighbors::sendHellos(int duration_ms)
{
    if (nullptr == routingProtocol)
    {
        std::cout << "[HELLO][ERROR]: HELLO routing protocol not set." << std::endl;
        return;
    }

    _updateNeighbors(duration_ms);
}

void HelloNeighbors::_updateNeighbors(int remaining_time_ms)
{
    // 0. Should we continue? 
    if (remaining_time_ms <= 0)
        return;

    // 1. Update neighbors of routing protocol
    // TODO: Should we add a mux/semaphore here? 
    routingProtocol->resetLinks();
    for (IP_ADDR link : m_neighbors)
    {
        routingProtocol->addLink(link);
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
    _updateNeighbors(remaining_time_ms - HELLO_INTERVAL_MS);
}

void HelloNeighbors::_broadcastHelloMessage()
{
    char* buffer = (char*)(malloc(4));
    memcpy(buffer, &m_parentIp, 4);

    if (HELLO_DEBUG)
        cout << "[DEBUG][HELLO]: Broadcasting hello message from node " << getStringFromIp(m_parentIp) << endl;

    // send data on HELLO port
    routingProtocol->sendPacket(getPortId(), buffer, 4, getIpFromString(BROADCAST_STR), m_parentIp);
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
        std::cout << "[HELLO][DEBUG]: Sleeping for " << DURATION_MS << " ms" << std::endl;
}
