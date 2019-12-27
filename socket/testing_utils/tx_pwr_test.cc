#include "../../socket/endpoint.h"
#include "../../socket/safe_circular_queue.h"
#include "../../socket/udp_socket.h"
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

#define SENDER 1


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

bool sendMessage(UDPSocket *socket, Endpoint &socket2End, string data1) {
    int maxAttempts = 2;
    if(SENDER){
        // Send data 
        int count = 0;
        do {
            count++;
            socket->sendTo(socket2End, &data1[0], data1.length());
            sleep(1);
        } while (count < maxAttempts);

        // Receive data
        string data2;
        Message message2to1;

        count = 0;
        do {
            count++;
            sleep(1);
        } while ((!socket->getMessage(message2to1)) && count < maxAttempts);
        
        // Check if sending was successful
        return (count < maxAttempts);
    }

     // Clear both sockets
     Message temp;
    do {
        sleep(1);
    } while (socket->getMessage(temp));
}

int main() {
    // Create socket 1
    int port1 = 8881;
    thread socket1ing;
    UDPSocket *socket1 = createThreadedSocket(socket1ing, port1);

    // Create endpoint for socket1
    Endpoint socket2End;
    int port2 = 8882;
    socket2End.setAddress("127.0.0.1", port2);

    if(SENDER){
        int txPower = 20;
        bool sent;
        do{ 
            socket1->setTransmissionPower(txPower);
            char message[60];
            sprintf(&message[0],
                    "Message sending with transmission power %d dBm \n",
                    txPower);
            sent = sendMessage(socket1, socket2End, message);
            txPower --;
        }while(sent);
    }else{
        Message message1to2;
        for(;;){
            sleep(1);
            if(socket1->getMessage(message1to2)){
                for(int i = 0; i < 2;i++){
                    socket1->sendTo(socket2End, "received", 9);
                }
                printf("Reveived: %s\n", message1to2.getData());
            }
        }
    }
    return 0;
}