// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>

#include "aodv/aodv.h"
#include "aodv/defines.h"
//#include "socket/endpoint.h"
#include "socket/udp_socket.h"
//#include "socket/message.h"

using namespace std;

struct config
{
    IP_ADDR deviceIP;
};

void getConfig(config* config);

int main()
{
    // get config details
    config cfg;
    getConfig(&cfg);

    // create an AODV object for this device 
    AODV aodv(cfg.deviceIP);

    // Create UDP socket
    UDPSocket *receiver = new UDPSocket();
    int port = 8080;
    if (!receiver->bindToPort(port)) {
        fprintf(stderr, "Could not bind the receiver socket to port:%d\n", port);
        exit(-1);
    }

    thread receiving(&UDPSocket::receiveFromPortThread, receiver);

    while (true) {
        Message message;
        if (receiver->getMessage(message)) {
//        printf("Received message from %s at port %d: %s\n", message.first.getAddress(), message.first.getPort(),
//                message.second);

            // pass packet to aodv 
            aodv.receivePacket(message.getData(), message.getLength(), message.getIPAddress());
        }
    }
    receiving.join();

    return 0;
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
