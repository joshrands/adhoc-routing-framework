// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>
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
    HardwareAODV haodv(inet_addr("138.67.203.229"));
//    HardwareAODV haodv2("138.67.194.210");

//    cout << getStringFromIp(haodv2.getIp()) << endl;
    cout << getStringFromIp(haodv.getIp()) << endl;
    cout << haodv.getIp() << " : " << inet_addr("1.1.168.192") << endl;

    string message = "Hello World!";
    cout << message.length() << endl;

    uint32_t dest = getIpFromString("127.0.0.1");
    haodv.sendPacket(&message[0], message.length(), dest);
    haodv.sendPacket(&message[0], message.length(), dest);

    while (-1 == haodv.handleReceivedPackets());
}

void getConfig(config* config)
{
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
