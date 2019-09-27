#include "send_packet.h"

IP_ADDR lastSource;
uint32_t globalPacketCount = 0;

extern void sendBuffer(char* buffer, int length, IP_ADDR source, IP_ADDR dest)
{
    #if (PHYSICAL_TESTBED)
    {

    }
    #else 
    {
        if (DEBUG)
            cout << "Sending buffer from " << getStringFromIp(source) << " to " << getStringFromIp(dest) << endl;

        lastSource = source;
        globalPacketCount++;
    }
    #endif
}

extern bool receivePacket(char* buffer, int length, IP_ADDR source, int port)
{
    #if (PHYSICAL_TESTBED)
    {
        // check receiving threads 
    }
    #else 
    {

    }
    #endif
}

IP_ADDR getLastSource()
{ 
    return lastSource;
}

uint32_t getGlobalPacketCount()
{
    return globalPacketCount;
}
