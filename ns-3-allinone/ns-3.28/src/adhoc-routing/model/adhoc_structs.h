#ifndef ADHOC_STRUCTS_H
#define ADHOC_STRUCTS_H

#include "stddef.h"
#include "defines.h"

struct BufferedPacket
{
	BufferedPacket()
	{
		this->dest = 0;
		this->portId = 0;
		this->buffer = NULL;
		this->length = 0;
	}

	BufferedPacket(const BufferedPacket& bufferedPacket)
	{
		this->dest = bufferedPacket.dest;
		this->portId = bufferedPacket.portId;
		this->buffer = bufferedPacket.buffer;
		this->length = bufferedPacket.length;
	}

	IP_ADDR dest;
	int portId;
	char* buffer;
	int length;
};

#endif
