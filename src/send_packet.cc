#include "send_packet.h"

IP_ADDR lastSource;
uint32_t globalPacketCount = 0;

extern void sendBuffer(char* buffer, int length, IP_ADDR source, IP_ADDR dest)
{
    if (DEBUG)
        cout << "Sending buffer from " << source << " to " << dest << endl;

    if (dest = getIpFromString("255.255.255.255"))
    {
        if (DEBUG)
            cout << "Broadcasting packet!" << endl;
    }

    lastSource = source;
    globalPacketCount++;
}

IP_ADDR getLastSource()
{ 
    return lastSource;
}

uint32_t getGlobalPacketCount()
{
    return globalPacketCount;
}
