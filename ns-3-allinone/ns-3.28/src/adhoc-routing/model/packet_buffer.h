#ifndef PACKET_BUFFER_H
#define PACKET_BUFFER_H

/*********************************
 * packet_buffer.h
 *
 * C++ implementation of a Packet Buffer
 *
 * Author: Jonathan Diller
 * Date: 08/17/2020
 ********************************/

#include "defines.h"
#include "adhoc_structs.h"

#include <iostream>
#include <queue>
#include <map>

class PacketBuffer
{
public:

	PacketBuffer();
	~PacketBuffer();

	// Stores packets in buffer
	void storePacket(BufferedPacket bufferedPacket);
	// Checks to see if there are packets waiting to be sent to dest
	bool packetsWaiting(IP_ADDR dest);
	// Pops off the top packet for dest
	BufferedPacket getPacket(IP_ADDR dest);

protected:

private:
	int m_nPacketCount;
	// map of rreq ids and their corresponding packet to be sent once the route is established
	std::map<IP_ADDR, std::queue<BufferedPacket>> m_oRREQPacketBuffer;
};
#endif
