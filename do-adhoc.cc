// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>
#include <string>
#include "hardware/hardware_aodv.h"

using namespace std;

struct config
{
    IP_ADDR deviceIP;
};

void getConfig(config* config);

int main(){
    // This might be a double negative right now...
    AODV::AODV_PORT = 13415;
    HardwareAODV haodv(getIpFromString("192.168.1.1"));
    string message = "Hello friend";

    while(true){
        printf("[INFO]: Sending message\n");
        uint32_t dest = getIpFromString("192.168.1.2");
        haodv.sendPacket(&message[0], message.length(), dest);
        haodv.sendPacket(&message[0], message.length(), dest);

        int aodvPackets = haodv.handleAODVPackets();
        if(aodvPackets < 0){
            printf("[INFO]: Have not received any AODV packages\n");
        }else{
            printf("[INFO]: Received %d AODV packet(s)\n", aodvPackets);
        }
        Message input;
        if(haodv.getDataPacket(input)){
            printf("[INFO]: Received %s from %d\n", input.getData(), input.getAddressI());
        }
        printf("_______________________________________________\n");
        sleep(1);
    }
}

void getConfig(config* config){
    // read config file 
    ifstream configFile;
    configFile.open("config");

    string line;
    if (configFile.is_open())
    {
        // get ip address
        getline(configFile, line);
        cout << line << endl;
        string ipString;
        ipString = line.substr(11, line.length() - 11);

        config->deviceIP = getIpFromString(ipString);

        configFile.close();
    }

}
