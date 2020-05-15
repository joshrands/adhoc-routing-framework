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
    Port* printPort = new PrintPort(DATA_PORT);
    haodv->addPort(printPort);

    // Light up all LEDs to avoid random lighting
    printf("[TEST ADHOC]:[DEBUG]: Cleaning LED pins...\n");
    lightLed(AODV_LED_PIN, 50);
    lightLed(HELLO_LED_PIN, 50);
    lightLed(OTHER_LED_PIN, 50);
    printf("[TEST ADHOC]:[DEBUG]: Cleaning LED pins...done\n");

    /// Networking Settings
    string message = "Hello World!";
    char* msg = strdup(message.c_str());

    vector<string> ips = { "192.168.1.2" };

    // Network
    std::chrono::milliseconds last_send_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    while(true){
        // Send packets to all ips every second
        std::chrono::milliseconds current_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

        if((current_time - last_send_time).count() > 1000){
            last_send_time = current_time;            
            for(auto ip : ips){
                uint32_t dest = getIpFromString(ip);
                if(!haodv->sendPacket(printPort->getPortId(), msg, message.length()+1, dest)){
                    fprintf(stderr, "[TEST ADHOC]:[ERROR]: Unable to send packet\n");
                    exit(1);
                }
                printf("[TEST ADHOC]:[DEBUG]: Sent ");
                printPacket(stdout, msg, message.length()+1);
                printf(" to %s\n", ip.c_str());
            }
        }

        // Handle packets
        int handleCount = haodv->handlePackets();
        if(handleCount > 0){
            printf("[TEST ADHOC]:[DEBUG]: Handled %d packets\n", handleCount);
        }
    }

    // Clean memory up
    delete printPort;
    delete haodv;
    delete msg;
}