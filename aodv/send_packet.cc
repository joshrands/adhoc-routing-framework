#include "send_packet.h"

IP_ADDR lastSource;
uint32_t globalPacketCount = 0;

int sendBuffer(char* buffer, int length, IP_ADDR dest, int port)
{
    if (DEBUG)
        cout << "Sending buffer from " << getStringFromIp(port) << " to " << getStringFromIp(dest) << endl;

//    lastSource = source;
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
