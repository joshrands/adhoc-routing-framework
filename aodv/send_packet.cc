#include "send_packet.h"

IP_ADDR lastSource;
uint32_t globalPacketCount = 0;
IP_ADDR lastReceive;

int sendBuffer(char* buffer, int length, IP_ADDR dest, int port)
{
    if (DEBUG)
        cout << "[DEBUG]: Sending buffer from " << getStringFromIp(port) << " to " << getStringFromIp(dest) << endl;

//    lastSource = source;
    incrementGlobalPacketCount();

		return length;
}

IP_ADDR getLastSource()
{ 
    return lastSource;
}

void setLastSource(IP_ADDR source)
{
    lastSource = source;
}

IP_ADDR getLastReceive()
{ 
    return lastReceive;
}

void setLastReceive(IP_ADDR receive)
{
    lastReceive = receive;
}

void incrementGlobalPacketCount()
{
    globalPacketCount++;
}

uint32_t getGlobalPacketCount()
{
    return globalPacketCount;
}
