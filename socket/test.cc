#include "endpoint.h"
#include "safe_circular_queue.h"
#include "udp_socket.h"
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <utility>
using namespace std;

/*!
 * @brief Create a Threaded Socket object
 *
 * @param returnThread the thread which the socket will be on
 * @param port the port to bind to
 * @return UDPSocket* the socket
 */
UDPSocket *createThreadedSocket(thread &returnThread, int port) {
    UDPSocket *threadedSocket = new UDPSocket();
    if (!threadedSocket->bindToPort(port)) {
        fprintf(stderr, "[ERROR]: Could not bind the receiver socket to port:%d\n",
                port);
    }
    // Thread a recvfrom call
    returnThread = thread(&UDPSocket::receiveFromPortThread, threadedSocket);
    returnThread.detach();
    return threadedSocket;
}

int main() {

    // Test messages can be sent
    {
        // Create receiving socket
        int port = 8080;
        // Bind to 8080
        thread receiving;
        UDPSocket *receiver = createThreadedSocket(receiving, port);
        // Create sending socket
        Endpoint sendLocation;
        sendLocation.setAddress("138.67.73.32", port);

        while(true){
            char message[7] = "Hellow";
            receiver->sendTo(sendLocation, message, 7);
            if(receiver->areThereMessages()){
                Message incoming;
                receiver->getMessage(incoming);
                printf("Message: %s dBm: %d\n", incoming.getData(), incoming.getRss());
                
            }
            sleep(1);
        }
    }

    return 0;
}