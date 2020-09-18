#include "packet_buffer.h"


PacketBuffer::PacketBuffer()
{
	m_nByteCount = 0;
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

	do
	{
		// Try to add packet
		if((m_nByteCount + bufferedPacket.length) <= PACKET_BUFFER_SIZE)
		{
			// Add packet
			m_oRREQPacketBuffer.at(bufferedPacket.dest).push(bufferedPacket);
			m_nByteCount += bufferedPacket.length;

			break;
		}
		else
		{
			// Buffer is full, try to remove older packet
			if(m_oRREQPacketBuffer.at(bufferedPacket.dest).size() > 0)
			{
				// Throw away oldest packet waiting to go to this destination
				m_nByteCount -= m_oRREQPacketBuffer.at(bufferedPacket.dest).front().length;
				free(m_oRREQPacketBuffer.at(bufferedPacket.dest).front().buffer);
				m_oRREQPacketBuffer.at(bufferedPacket.dest).pop();
			}
			else
			{
				// Buffer is full and there are no old packets to throw away for this destination
				// Just throw away this packet
				free(bufferedPacket.buffer);
				break;
			}
		}
	} while (true);
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
			m_nByteCount -= returnPacket.length;
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
