#include "packet_buffer.h"


PacketBuffer::PacketBuffer()
{
	m_nPacketCount = 0;
}


PacketBuffer::~PacketBuffer()
{
	for(auto it = m_oRREQPacketBuffer.begin(); it != m_oRREQPacketBuffer.end(); ++it)
	{
		// remove all packets from this queue
		std::queue<BufferedPacket> packetQueue = it->second;
		while(packetQueue.size() > 0)
		{
			free(packetQueue.front().buffer);
			packetQueue.pop();
		}
	}
}

/******************************
 * Public Functions
 ******************************/

void PacketBuffer::storePacket(BufferedPacket bufferedPacket)
{
	// Check if there is a queue for this address
	if(!(m_oRREQPacketBuffer.count(bufferedPacket.dest) > 0))
	{
		m_oRREQPacketBuffer.insert(std::pair<IP_ADDR, std::queue<BufferedPacket>>(bufferedPacket.dest, std::queue<BufferedPacket>()));
	}

	if(m_nPacketCount < PACKET_BUFFER_SIZE)
	{
		m_oRREQPacketBuffer.at(bufferedPacket.dest).push(bufferedPacket);
		m_nPacketCount++;
	}
	else
	{
		// Throw away oldest packet waiting to go to this destination
		free(m_oRREQPacketBuffer.at(bufferedPacket.dest).front().buffer);
		m_oRREQPacketBuffer.at(bufferedPacket.dest).pop();

		// Save newest packet
		m_oRREQPacketBuffer.at(bufferedPacket.dest).push(bufferedPacket);
	}
}

bool PacketBuffer::packetsWaiting(IP_ADDR dest)
{
	if(m_oRREQPacketBuffer.count(dest) > 0)
	{
		return (m_oRREQPacketBuffer.at(dest).size() > 0);
	}
	else
	{
		return false;
	}
}

BufferedPacket PacketBuffer::getPacket(IP_ADDR dest)
{
	BufferedPacket returnPacket;

	if(m_oRREQPacketBuffer.count(dest) > 0)
	{
		if(m_oRREQPacketBuffer.at(dest).size() > 0)
		{
			returnPacket = BufferedPacket(m_oRREQPacketBuffer.at(dest).front());
			m_oRREQPacketBuffer.at(dest).pop();
			m_nPacketCount--;
		}
	}

	return returnPacket;
}

/******************************
 * Protected Functions
 ******************************/


/******************************
 * Private Functions
 ******************************/
