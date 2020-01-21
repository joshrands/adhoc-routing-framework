#include "hello_monitor.h"

HelloNeighbors::HelloNeighbors()
{

}

HelloNeighbors::HelloNeighbors(IP_ADDR parentIp)
{

}

HelloNeighbors::~HelloNeighbors()
{

}

void HelloNeighbors::updateNeighbors()
{
    // 1. Update neighbors of routing protocol
    // 2. Clear neighbors for next time period 
    // 3. Wait 1/2 hello interval
    // 4. Broadcast hello message  
    // 5. Update neighbors from received hellos 
    // 6. Wait 1/2 hello interval
    // 7. Repeat
}

void HelloNeighbors::broadcastHelloMessage()
{

}

void HelloNeighbors::receiveHelloMessage(IP_ADDR nodeIp)
{
    
}
