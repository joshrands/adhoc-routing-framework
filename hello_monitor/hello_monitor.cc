#include "hello_monitor.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

int HelloMonitor::HELLO_INTERVAL_MS = 1000;

HelloMonitor::~HelloMonitor()
{

}

void HelloMonitor::handlePacket(char* data, int length, IP_ADDR source)
{
    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: Hello packet received! ADDING NEIGHBOR" << endl;

    // if a hello message was received from SOURCE, then that is a neighbor! 
    receiveHelloMessage(source);
} 

void HelloMonitor::sendHellos(int duration_ms)
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

void HelloMonitor::_updateNeighbors(int remaining_time_ms)
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

    // update the active neighbors 
    // check the at risk neighbors. if an at risk neighbor is not in the detected neighbors then remove it 
    for (IP_ADDR atRiskLink : m_atRiskNeighbors)
    {
        if (m_detectedNeighbors.count(atRiskLink) > 0)
        {
            // the link was detected! no longer at risk. 
            m_atRiskNeighbors.erase(atRiskLink);
        }
        else 
        {
            // the link was not detected. remove the link from active links and at risk links 
            m_activeNeighbors.erase(atRiskLink);
            m_atRiskNeighbors.erase(atRiskLink);
            if (HELLO_DEBUG)
                cout << "[HELLO]:[INFO]: Link " << getStringFromIp(atRiskLink) << " removed from "
                     << getStringFromIp(routingProtocol->getIp()) << endl;
        }
    }

    // check the active links and add at risk links if not detected 
    for (IP_ADDR activeLink : m_activeNeighbors)
    {
        if (m_detectedNeighbors.count(activeLink) == 0)
        {
            // the link was not detected... move to at risk link 
            m_atRiskNeighbors.insert(activeLink);
        }
    }

    // 1. Update neighbors of routing protocol
    routingProtocol->neighborMux.lock();
    routingProtocol->resetLinks();

    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: Adding " << m_activeNeighbors.size() << " neighbors." << endl;

    for (IP_ADDR link : m_activeNeighbors)
    {
        if (HELLO_DEBUG)
            cout << "[HELLO]:[INFO]: Add link to node " << getStringFromIp(link) << endl;

        routingProtocol->addLink(link);
    }

    routingProtocol->neighborMux.unlock();

    // 2. Clear neighbors for next time period 
    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: CLEARING ALL LOCAL HELLO NEIGHBORS." << endl;

    // clear detected neighbors for this round 
    m_detectedNeighbors.clear();

    m_activeNeighbors.clear();

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
    // if this neighbor doesn't already exist, add it to the vector 
    helloMux.lock();

    if (HELLO_DEBUG)
        cout << "[HELLO]:[DEBUG]: Adding neighbor " << getStringFromIp(nodeIp) 
             << " to node " << getStringFromIp(routingProtocol->getIp()) << endl;

    routingProtocol->neighborMux.lock();
    routingProtocol->addLink(nodeIp);
    routingProtocol->neighborMux.unlock();

    if (m_activeNeighbors.count(nodeIp) == 0)
        m_activeNeighbors.insert(nodeIp);

    if (m_detectedNeighbors.count(nodeIp) == 0)
        m_detectedNeighbors.insert(nodeIp);

    helloMux.unlock();
}

bool HelloTest::_sleep(int DURATION_MS)
{
    if (HELLO_DEBUG)
        std::cout << "[HELLO][DEBUG]: Sleeping for " << DURATION_MS << " ms" << std::endl;

    // wait sleep time 
    this_thread::sleep_for(chrono::milliseconds(DURATION_MS));

    return true;
}

void dispatchHello(HelloTest* hello, int duration)
{
    if (HELLO_DEBUG)
        cout << "[DEBUG]:[HELLO]: Dispatching hello sendHellos" << endl;

    hello->sendHellos(duration);
}
