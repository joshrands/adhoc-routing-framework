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
#include "hardware_hello_aodv.h"
#include "hardware_rssi.h"

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
bool testHardwareRSSI();
    bool testCollectAll();
    bool testSelective();

int main() {
    bool allPass = true;
    // Basic testing of hardwareAODV
    allPass &= testHardwareAodv();
    printf("________________________________\n\n");
    // Testing of hardware hello
    allPass &= testHardwareHello();
    printf("________________________________\n\n");
    // Testing of hardware rssi
    allPass &= testHardwareRSSI();
    printf("________________________________\n\n");
    printf("HARWARE TESTS COMPLETE\n");
    return allPass;
}


bool testHardwareAodv(){
    bool pass = true;

    RoutingProtocol* haodv = new HardwareHelloAODV("127.0.0.1", 3000);
    PrintPort* printPort = new PrintPort(DATA_PORT);
    haodv->addPort(printPort);

    char *msg = (char *)(malloc(17));
    string message = "Hello World!";
    memcpy(msg, message.c_str(), 17);

    bool send = true;
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.1"));
    send = send && haodv->sendPacket(printPort->getPortId(), msg, 16, getIpFromString("127.0.0.1"));
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

    return pass;
}


bool testHardwareHello(){
    RoutingProtocol* basicRoutingProt = new AODVTest(getIpFromString("127.0.0.1"));
    HHMTest* hello = new HHMTest(HELLO_PORT, basicRoutingProt);
    printf("current time: %lu \n", hello->getCurrentTimeMS());
    
    uint64_t start = hello->getCurrentTimeMS();
    hello->sleep(100);
    uint64_t end = hello->getCurrentTimeMS();

    uint64_t elapsed = end-start;
    return test((elapsed > 98 && elapsed < 102), std::string("Hardware hello waits properly"));
}

bool testHardwareRSSI(){
    bool allPass = true;
    allPass &= testCollectAll();
    allPass &= testSelective();
    return allPass;
}

bool testCollectAll(){
    HardwareRSSI* hRSSI = new HardwareRSSI(true);
    thread capturing = thread(&HardwareRSSI::captureData, hRSSI);
    capturing.detach();

    sleep(2);

    delete hRSSI;
    return test(hRSSI->count > 1, string("Collecting all RSSI data works properly"));
}

bool testSelective(){
    bool allPass = true;

    HardwareRSSI* hRSSI = new HardwareRSSI(false);
    thread capturing = thread(&HardwareRSSI::captureData, hRSSI);
    capturing.detach();

    sleep(2);
    allPass &= test(hRSSI->count == 0, string("Selective Hardware RSSI doesn't collect any data when no neighbors are provided"));

    const char* mac = "7c:9a:54:ea:8e:97";
    hRSSI->addNeighbors(mac, getIpFromString("192.168.1.2"));

    sleep(2);
    allPass &= test(hRSSI->count > 0, string("Selective Hardware RSSI collects rssi data when Zach's Wifi router is near by"));


    delete hRSSI;
    return allPass;
}
