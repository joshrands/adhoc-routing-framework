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

void test(bool condition, char *desc) {
    if (condition) {
        cout << GREEN << "[PASS]: " << desc << endl;
    } else
        cout << RED << "[FAIL]: " << desc <<endl;
}


int main() {
    // Test initialization and ports
    {
        HardwareAODV haodv(getIpFromString("127.0.0.1"));
        PrintPort* printPort = new PrintPort(DATA_PORT);
        haodv.addPort(printPort);

        char *msg = (char *)(malloc(16));
        string message = "Hello World!";
        memcpy(msg, &message, 16);

        haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        haodv.sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        haodv.handlePackets();
        delete printPort;
    }

    printf("________________________________\n\n");
    // Test messages can be sent
    {
        RoutingProtocol* haodv = new HardwareAODV(getIpFromString("127.0.0.1"));
        PrintPort* printPort = new PrintPort(DATA_PORT);
        haodv->addPort(printPort);

        char *msg = (char *)(malloc(16));
        string message = "Hello World!";
        memcpy(msg, &message, 16);

        haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
        haodv->handlePackets();
        haodv->removePort(printPort);
        haodv->handlePackets();
        delete printPort;
        delete haodv;

    }

    printf("________________________________\n\n");
    // Test communication between sockets
    {}
    return 0;
}