#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <utility>

#include "../aodv/test.h"
#include "hardware_aodv.h"
#include "hardware_hello.h"
#include "hardware_aodv_hello.h"

using namespace std;

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string END = "\033[0m\n";

bool test(bool condition, string desc)
{
	if (condition)
		cout << GREEN << "[PASS]: " << desc << END;
	else 
		cout << RED << "[FAIL]: " << desc << END;
    
    return condition;
}

bool testHardwareAodv();
bool testHardwareHello();
    bool testMS();
    bool testWait();

int main() {
    bool allPass = true;
    // Basic testing of hardwareAODV
    allPass &= testHardwareAodv();
    printf("________________________________\n\n");
    // Testing of hardware hello
    allPass &= testHardwareHello();
    printf("________________________________\n\n");
    printf("HARWARE TESTS COMPLETE\n");
    return allPass;
}


bool testHardwareAodv(){
    bool pass = true;

    RoutingProtocol* haodv = new HardwareHelloAODV(getIpFromString("127.0.0.1"), 3000);
    PrintPort* printPort = new PrintPort(DATA_PORT);
    haodv->addPort(printPort);

    char *msg = (char *)(malloc(16));
    string message = "Hello World!";
    memcpy(msg, &message, 16);

    bool send = true;
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.2"));
    pass &= test(send, "Can attempt to send message to node on network");
    sleep(1);
    pass &= test(4 == haodv->handlePackets(), "RREQ are broadcasted and handled");
    send = true;
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.3"));
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.3"));
    pass &= test(send, "Can attempt to send message to node on network");
    sleep(1);
    pass &= test(4 == haodv->handlePackets(), "RREQ are broadcasted and handled");
    send = true;
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.9"));
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.9"));
    pass &= test(send, "Can attempt to send message to node on network");
    sleep(1);
    pass &= test(4 == haodv->handlePackets(), "RREQ are broadcasted and handled");
    haodv->removePort(printPort);
    pass &= test(0 == haodv->handlePackets(), "Messages aren't sent mutiple times");
    
    delete printPort;
    delete haodv;

    return !pass;
}


bool testHardwareHello(){
    return testMS() && testWait();
}

bool testMS(){
    RoutingProtocol* basicRoutingProt = new AODVTest(getIpFromString("127.0.0.1"));
    HHMTest hello(HELLO_PORT, basicRoutingProt);
    printf("current time: %lu \n", hello.getCurrentTimeMS());
    delete basicRoutingProt;
    return true;
}

bool testWait(){
    RoutingProtocol* basicRoutingProt = new AODVTest(getIpFromString("127.0.0.1"));
    HHMTest hello(HELLO_PORT, basicRoutingProt);
    
    uint64_t start = hello.getCurrentTimeMS();
    hello.sleep(100);
    uint64_t end = hello.getCurrentTimeMS();

    uint64_t elapsed = end-start;
    delete basicRoutingProt;
    return test((elapsed > 98 && elapsed < 102), std::string("Hardware hello waits properly"));
}