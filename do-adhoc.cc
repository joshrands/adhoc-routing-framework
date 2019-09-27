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

    // add function call to send data on aodv socket 
//    aodv.socketSendPacket = &(receiver->sendTo);

    thread receiving(&UDPSocket::receiveFromPortThread, receiver);
    receiving.detach();

    cout << getIpFromString("127.0.0.1") << endl;

    while (true) {
        cout << "Choose an option: " << endl << "1. Check received message queue" << endl << "2. Send a packet" << endl;

        int opt;
        cin >> opt;
        Message message;
       
        switch (opt) {
            case 1: 
                if (receiver->getMessage(message)) {
                    // pass packet to aodv 
                    aodv.receivePacket(message.getData(), message.getLength(), message.getIPAddress());
                }
                else 
                    cout << "No packets." << endl;
                break;
            case 2:
                string ip;
                cout << "Enter ip address to send to: ";
                cin >> ip;
                char* packet = "Hello world! From node 0";
            	int length = sizeof("Hello world! From node 0");
                aodv.sendPacket(packet, length, getIpFromString(ip));

                cout << "Packet sent!" << endl;
                break;
        }
        cout << endl;
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
