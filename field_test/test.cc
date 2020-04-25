// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>

#include "hardware_led_aodv.h"
#include "aodv_test.h"

using namespace std;

int main(){
    /// Setup
    RoutingProtocol* haodv = new HardwareLedAODV(MY_IP_ADDR);
    PrintPort* printPort = new PrintPort(DATA_PORT);
    haodv->addPort(printPort);

    // Light up all LEDs to avoid random lighting
    printf("[TEST ADHOC]:[DEBUG]: Cleaning LED pins...\n");
    std::chrono::milliseconds start = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    lightLed(AODV_LED_PIN, 50);
    lightLed(HELLO_LED_PIN, 50);
    lightLed(OTHER_LED_PIN, 50);
    std::chrono::milliseconds end = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    printf("[TEST ADHOC]:[DEBUG]: Cleaning LED pins...done\n");
    printf("AHH: time %ld ms\n", (end-start).count());

    /// Networking Settings
    string message = "Hello World!";
    char* msg = strdup(message.c_str());

    vector<string> ips = { "192.168.1.1" };

    // Network
    while(true){
        // Send packets to all ips
        for(auto ip : ips){
            uint32_t dest = getIpFromString(ip);
            if(!haodv->sendPacket(printPort->getPortId(), msg, message.length()+1, dest)){
                fprintf(stderr, "[TEST ADHOC]:[ERROR]: Muy angry send, much mad\n");
            }
            printf("[TEST ADHOC]:[DEBUG]: Sent ");
            printPacket(stdout, msg, message.length()+1);
            printf(" to %s\n", ip.c_str());
        }

        // Handle packets
        int handleCount = haodv->handlePackets();
        if(handleCount > 0){
            printf("[TEST ADHOC]:[DEBUG]: Handled %d packets\n", handleCount);
        }

        sleep(1);
    }

    // Clean memory up
    delete printPort;
    delete haodv;
}