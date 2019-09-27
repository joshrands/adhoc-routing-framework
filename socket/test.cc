#include "endpoint.h"
#include "udp_socket.h"
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include <time.h>
using namespace std;

void test(bool condition, char *message) {
  printf("%s: ", message);
  assert(condition);
  cout << "PASS" << endl;
}

void test(bool condition, string message) {
  cout << message << ": ";
  assert(condition);
  cout << "PASS" << endl;
}

void sendMessageToThreadedServerTest(UDPSocket *server, UDPSocket &sender,
                                     Endpoint &serverEnd, string data) {
  // Continue to send data until received
  pair<Endpoint, char *> message;
  int count = 0;
  const int MAX_ATTEMPTS = 2;
  do {
    count++;
    sender.sendTo(serverEnd, &data[0], data.length());
    sleep(1);
  } while ((!server->getMessage(message) && count < MAX_ATTEMPTS));
  // Check if test was successful
  char maxAttemptsMessage[60];
  sprintf(&maxAttemptsMessage[0],
          "Can send message to threaded socket with < %d attempts",
          MAX_ATTEMPTS);
  test(count < MAX_ATTEMPTS, &maxAttemptsMessage[0]);
  string messageSentMessage = "Can send '" + data + "' to threaded socket";
  test(strcmp(message.second, &data[0]) == 0, messageSentMessage);

  // Clear the servers queue
  do {
    sleep(1);
  } while (server->getMessage(message));
}

void sendMessageBetweenThreadedSockets(UDPSocket* socket1, UDPSocket* socket2,
                                     Endpoint &socket2End, string data1, string data2) {
  // Send data from 1 to 2
  pair<Endpoint, char *> message1to2;
  int count = 0;
  const int MAX_ATTEMPTS = 2;
  do {
    count++;
    socket1->sendTo(socket2End, &data1[0], data1.length());
    sleep(1);
  } while ((!socket2->getMessage(message1to2) && count < MAX_ATTEMPTS)); 
  // Check if test was successful
  char maxAttemptsMessage[60];
  sprintf(&maxAttemptsMessage[0],
          "Can send message to another socket with < %d attempts",
          MAX_ATTEMPTS);
  test(count < MAX_ATTEMPTS, &maxAttemptsMessage[0]);
  string messageSentMessage = "Socket 1 sent '" + data1 + "' to socket 2";
  test(strcmp(message1to2.second, &data1[0]) == 0, messageSentMessage);
  
  // Send data from 2 to 1
  pair<Endpoint, char *> message2to1;
  count = 0;
  do {
    count++;
    socket2->sendTo(message1to2.first, &data2[0], data2.length());
    sleep(1);
  } while ((!socket1->getMessage(message2to1) && count < MAX_ATTEMPTS)); 
  // Check if test was successful
  sprintf(&maxAttemptsMessage[0],
          "Can send message to another socket with < %d attempts",
          MAX_ATTEMPTS);
  test(count < MAX_ATTEMPTS, &maxAttemptsMessage[0]);
  messageSentMessage = "Socket 2 sent '" + data2 + "' to socket 1";
  test(strcmp(message2to1.second, &data2[0]) == 0, messageSentMessage);

  // Clear both sockets
  do {
    sleep(1);
  } while (socket1->getMessage(message2to1) || socket2->getMessage(message1to2));
}

int main() {
  // Test initialization and ports
  {
    UDPSocket nonboundSocket;
    test(nonboundSocket.init(),
         (string) "Can initialize noninitialized socket");
    test(!nonboundSocket.init(),
         (string) "Cannot reinitialize previously initialized socket");
    test(!nonboundSocket.bindToPort(8080),
         (string) "Cannot bind previously initialized socket to a port");
    UDPSocket boundSocket;
    test(boundSocket.bindToPort(8080), (string) "Can bind socket to port");
    test(!nonboundSocket.init(),
         (string) "Cannot reinitialize socket that is bound to a port");
    test(!nonboundSocket.bindToPort(8070),
         (string) "Cannot bind socket that is already bound to a port");
  }

  // Test timeout
  /*
  {
    UDPSocket timeoutSocket;
    timeoutSocket.init();
    timeoutSocket.setBlocking(false, 500);
    char messageTemp [MAXLINE];
    Endpoint endTemp;
    time_t start = time(NULL);
    test(timeoutSocket.receiveFrom(endTemp, &messageTemp[0], MAXLINE)==0, (string)"Nonblocking socket will time out");
    time_t end = time(NULL);
    test((end-start)> 1 && (end-start)< 2, (string)"Timeout socket waits the appropritate time");

  }
  */
  printf("________________________________\n\n");
  // Test messages can be sent
  {
    // Create receiving socket
    UDPSocket *receiver = new UDPSocket();
    int port = 8080;
    // Bind to 8080
    if (!receiver->bindToPort(port)) {
      fprintf(stderr, "Could not bind the receiver socket to port:%d\n", port);
      exit(-1);
    }
    // Thread a recvfrom call
    thread receiving(&UDPSocket::receiveFromPortThread, receiver);
    receiving.detach();
    // Create sending socket
    UDPSocket sender;
    sender.init();
    Endpoint sendLocation;
    sendLocation.setAddress("127.0.0.1", port);

    // Send a few messages
    sendMessageToThreadedServerTest(receiver, sender, sendLocation, "hello");
    sendMessageToThreadedServerTest(
        receiver, sender, sendLocation,
        "Weird little people are eating the bakery");
    sendMessageToThreadedServerTest(receiver, sender, sendLocation,
                                    "120.1.7.9:80192 Little Node Buffer Size");
    sendMessageToThreadedServerTest(receiver, sender, sendLocation,
                                    "INFINITE NUMBER HAHAHA");
    receiving.~thread();
  }

  printf("________________________________\n\n");
  // Test communication between sockets
  {
    // Create socket 1
    UDPSocket *socket1 = new UDPSocket();
    int port1 = 8080;
    // Bind to 8080
    if (!socket1->bindToPort(port1)) {
      fprintf(stderr, "Could not bind the receiver socket to port:%d\n", port1);
      exit(-1);
    }
    // Thread a recvfrom call
    thread socket1ing(&UDPSocket::receiveFromPortThread, socket1);
    socket1ing.detach();

    // Create socket 2
    UDPSocket *socket2 = new UDPSocket();
    int port2 = 8060;
    // Bind to 8080
    if (!socket2->bindToPort(port2)) {
      fprintf(stderr, "Could not bind the receiver socket to port:%d\n", port2);
      exit(-1);
    }
    // Thread a recvfrom call
    thread socket2ing(&UDPSocket::receiveFromPortThread, socket2);
    socket2ing.detach();

    // Create endpoint for socket1
    Endpoint socket2End;
    socket2End.setAddress("127.0.0.1", port2);
    
    // Send a few messages between them
    sendMessageBetweenThreadedSockets(socket1, socket2, socket2End, "Hello socket 2 how are you today?", "Doing well socket 1, doing well.");
    sendMessageBetweenThreadedSockets(socket1, socket2, socket2End, "Play us a song your the piano man", "Sing us a song tonight");
    sendMessageBetweenThreadedSockets(socket1, socket2, socket2End, "Well were all in the mood for a melody", "And you got us feeling all right");
  }

  return 0;
}