#include "../socket/udp_socket.h"
#include "../socket/message.h"
#include <thread>

//g++ -o pi pi.cc  -std=c++11 -pthread ../socket/libsocket.a 

/*!
 * @brief Create a Threaded Socket object
 * 
 * @param returnThread the thread which the socket will be on
 * @param port the port to bind to
 * @return UDPSocket* the socket
 */
UDPSocket* createThreadedSocket(thread& returnThread, int port){
  UDPSocket *threadedSocket = new UDPSocket();
  if (!threadedSocket->bindToPort(port)) {
      fprintf(stderr, "Could not bind the receiver socket to port:%d\n", port);
      exit(-1);
    }
    // Thread a recvfrom call
    returnThread = thread(&UDPSocket::receiveFromPortThread, threadedSocket);
    returnThread.detach();
    return threadedSocket;
}

int main(){
    // Create socket
    int port = 50518;
    int other_port = 50518;
    thread socketing;
    UDPSocket* socket = createThreadedSocket(socketing, port);
    socket->setBroadcasting(true);
    while(true){
        // Send a message
        string theData = "some data";
        socket->sendTo(&theData[0], theData.length(), inet_addr("192.168.1.1"), other_port);
        printf("Sending data...\n");
        sleep(1);

        // read data
        Message incoming;
        if(socket->getMessage(incoming)){
            printf("Received data: %s\n", incoming.getData());

        }else{
            printf("Have not received any messages yet\n");
        }
        sleep(1);
    }    
}
