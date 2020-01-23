#include "hardware_aodv.h"
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <utility>
#include "../aodv/test.h"

using namespace std;

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string END = "\033[0m\n";

void test(bool condition, string desc)
{
	if (condition)
		cout << GREEN << "[PASS]: " << desc << END;
	else 
		cout << RED << "[FAIL]: " << desc << END;
}

int main() {
    // Basic testing using pointer
    {
        RoutingProtocol* haodv = new HardwareAODV(getIpFromString("127.0.0.1"));
        PrintPort* printPort = new PrintPort(DATA_PORT);
        haodv->addPort(printPort);

        char *msg = (char *)(malloc(16));
        string message = "Hello World!";
        memcpy(msg, &message, 16);

        bool send = true;
        send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        test(send, "Can attempt to send message to node on network");
        sleep(1);
        test(2 == haodv->handlePackets(), "RREQ are broadcasted and handled");
        send = true;
        send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.3"));
        send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.3"));
        test(send, "Can attempt to send message to node on network");
        sleep(1);
        test(2 == haodv->handlePackets(), "RREQ are broadcasted and handled");
        send = true;
        send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.9"));
        send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.9"));
        test(send, "Can attempt to send message to node on network");
        sleep(1);
        test(2 == haodv->handlePackets(), "RREQ are broadcasted and handled");
        haodv->removePort(printPort);
        test(0 == haodv->handlePackets(), "Messages aren't sent mutiple times");
        
        delete printPort;
        delete haodv;
    }
    printf("________________________________\n\n");
    // testing using class itself
    {   
        sleep(1); // wait for sockets to free up and everything
        HardwareAODV haodv(getIpFromString("127.0.0.1"));
        PrintPort* printPort = new PrintPort(DATA_PORT);
        haodv.addPort(printPort);

        char *msg = (char *)(malloc(16));
        string message = "Hello World!";
        memcpy(msg, &message, 16);

        bool send = true;
        send = send && haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        send = send && haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        test(send, "Can attempt to send message to node on network");
        sleep(1);
        test(2 == haodv.handlePackets(), "RREQ are broadcasted and handled");
        send = true;
        send = send && haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.3"));
        send = send && haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.3"));
        test(send, "Can attempt to send message to node on network");
        sleep(1);
        test(2 == haodv.handlePackets(), "RREQ are broadcasted and handled");
        send = true;
        send = send && haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.9"));
        send = send && haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.9"));
        test(send, "Can attempt to send message to node on network");
        sleep(1);
        test(2 == haodv.handlePackets(), "RREQ are broadcasted and handled");
        haodv.removePort(printPort);
        test(0 == haodv.handlePackets(), "Messages aren't sent mutiple times");
        delete printPort;
    }

    printf("________________________________\n\n");
    {}
    return 0;
}