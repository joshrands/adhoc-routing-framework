#include "aodv.h"

AODV::AODV()
{
	cout << "Created new aodv routing protocol." << endl;
}

RREQ AODV::createRREQ(const IP_ADDR dest)
{
	RREQ rreq; 

	// populate fields of rreq 

	return rreq;
}

void forwardRREQ(const RREQ receivedRREQ)
{

}

char* getRREQBuffer(const RREQ rreq)
{
	char* buffer = (char*)(malloc(sizeof(rreq)));	

	return buffer;
}

RREQ readRREQBuffer(char* buffer)
{
	RREQ rreq;

	return rreq;
}

RREP createRREP(const IP_ADDR dest)
{

}

void forwardRREP(const RREP receivedRREP)
{

}

char* createRREPBuffer(const RREP rrep)
{

}

RREP readRREPBuffer(char* buffer)
{

}

RERR createRERR(const IP_ADDR dest)
{

}

void forwardRERR(const RERR receivedRERR)
{

}

char* createRERRBuffer(const RERR rerr)
{

}

RERR readRERRBuffer(char* buffer)
{

}
