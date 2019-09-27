// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>

#include "aodv/aodv.h"
#include "aodv/defines.h"
#include "socket/endpoint.h"
#include "socket/udp_socket.h"

using namespace std;

int main()
{
    // create an AODV object 
    AODV aodv(getIpFromString("192.168.1.1"));

    // Create UDP socket
    UDPSocket *receiver = new UDPSocket();
    int port = 8080;
    if (!receiver->bindToPort(port)) {
        fprintf(stderr, "Could not bind the receiver socket to port:%d\n", port);
        exit(-1);
    }

    thread receiving(&UDPSocket::receiveFromPortThread, receiver);

    while (true) {
        pair<Endpoint, char *> message;
        if (receiver->getMessage(message)) {
        printf("Received message from %s at port %d: %s\n", message.first.getAddress(), message.first.getPort(),
                message.second);

//            aodv.receivePacket(message.second, 10, message.first.getAddress());
        }
    }
    receiving.join();

    return 0;
}

