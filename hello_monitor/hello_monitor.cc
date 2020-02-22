#include "hello_monitor.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>


void HelloMonitor::handlePacket(char* data, int length, IP_ADDR source)
{
    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: Hello packet received! ADDING NEIGHBOR" << endl;

    // if a hello message was received from SOURCE, then that is a neighbor! 
    receiveHelloMessage(source);
} 

void HelloMonitor::sendHellos(uint64_t duration_ms)
{
    if (nullptr == routingProtocol)
    {
        std::cout << "[HELLO][ERROR]: HELLO routing protocol not set." << std::endl;
        return;
    }

    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: Dispatching hello neighbors updating for node "
             << getStringFromIp(routingProtocol->getIp()) << endl;

    _updateNeighbors(duration_ms);
}

void HelloMonitor::_updateNeighbors(uint64_t remaining_time_ms)
{
    // 0. Should we continue? 
    helloMux.lock();

    if (remaining_time_ms <= 0)
    {
        m_active = false;
        helloMux.unlock();
        return;
    }

    // this hello neighbors object is still running, yay! 
    m_active = true;

    // 1. Update neighbors of routing protocol
    routingProtocol->neighborMux.lock();
    routingProtocol->resetLinks();

    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: Checking " << m_neighborDetectionTimes.size() << " neighbors." << endl;

    uint64_t currentTimeMS = _getCurrentTimeMS();

    std::map<IP_ADDR, uint64_t>::iterator it = m_neighborDetectionTimes.begin();

    // add valid neighbor links 
    while (it != m_neighborDetectionTimes.end())
    {
        if ((currentTimeMS - it->second) < HELLO_NEIGHBOR_TTL_MS)
        {
            if (HELLO_DEBUG)
                cout << "[HELLO]:[INFO]: Add link to node " << getStringFromIp(it->first) << endl;

            routingProtocol->addLink(it->first);
        }
        else if (HELLO_DEBUG)
        {
            cout << "[HELLO]:[INFO]: Invalid link from " << getStringFromIp(m_parentIp) << " to " << getStringFromIp(it->first) << endl; 
            cout << "[HELLO]:[DEBUG]: " << currentTimeMS - it->second << " ms old" << endl;
        }

        it++;
    }

    routingProtocol->neighborMux.unlock();

    // 2. Clear neighbors for next time period 
    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: CLEARING ALL LOCAL HELLO NEIGHBORS." << endl;

    // neighbors have been updated so we can unlock mutex 
    helloMux.unlock();

    // 3. Wait 1/2 hello interval
    _sleep(HELLO_INTERVAL_MS / 2);

    // 4. Broadcast hello message  
    _broadcastHelloMessage();

    // 5. Update neighbors from received hellos 
    // this is happening on a different thread in handlePackets :)

    // 6. Wait 1/2 hello interval
    _sleep(HELLO_INTERVAL_MS / 2);

    // 7. Repeat
    _updateNeighbors(remaining_time_ms - HELLO_INTERVAL_MS);
}

void HelloMonitor::_broadcastHelloMessage()
{
    char* buffer = (char*)(malloc(4));
    memcpy(buffer, &m_parentIp, 4);

    if (HELLO_DEBUG)
        cout << "[DEBUG]:[HELLO]: Broadcasting hello message from node " << getStringFromIp(m_parentIp) << endl;

    // send data on HELLO port
    routingProtocol->sendPacket(getPortId(), buffer, 4, getIpFromString(BROADCAST_STR), m_parentIp);
}

void HelloMonitor::receiveHelloMessage(IP_ADDR nodeIp)
{
    // if this neighbor doesn't already exist, add it to the map 
    helloMux.lock();

    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: Adding neighbor " << getStringFromIp(nodeIp) 
             << " to node " << getStringFromIp(routingProtocol->getIp()) << endl;

    // add this link in real time so we don't have to wait an entire hello monitor cycle.
    routingProtocol->neighborMux.lock();
    routingProtocol->addLink(nodeIp);
    routingProtocol->neighborMux.unlock();

    // update local map detection time
    m_neighborDetectionTimes[nodeIp] = _getCurrentTimeMS();

    helloMux.unlock();
}


bool HelloTest::_sleep(uint64_t duration_ms)
{
    if (HELLO_DEBUG)
        std::cout << "[HELLO][DEBUG]: Sleeping for " << duration_ms << " ms" << std::endl;

    // wait sleep time 
    for (uint64_t i = 0; i < duration_ms; i+=duration_ms/10)
    {
        m_clockMS += i;
        this_thread::sleep_for(chrono::milliseconds(i));
    }

    return true;
}

uint64_t HelloTest::_getCurrentTimeMS()
{
    return m_clockMS;
}

void dispatchHello(HelloTest* hello, uint64_t duration)
{
    if (HELLO_DEBUG)
        cout << "[DEBUG]:[HELLO]: Dispatching hello sendHellos" << endl;

    hello->sendHellos(duration);
} 
