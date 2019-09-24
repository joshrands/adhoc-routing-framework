#include "endpoint.h"
#include "udp_socket.h"
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
using namespace std;

void test(bool condition, char *message) {
  printf("%s: ", message);
  assert(condition);
  cout << "PASS" << endl;
}

void test(bool condition, string message) {
  cout<<message<<": ";
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

int main() {
  // Test initialization and ports
  {
    UDPSocket nonboundSocket;
    test(nonboundSocket.init(), (string)"Can initialize noninitialized socket");
    test(!nonboundSocket.init(), (string)"Cannot reinitialize previously initialized socket");
    test(!nonboundSocket.bindToPort(8080), (string)"Cannot bind previously initialized socket to a port");
    UDPSocket boundSocket;
    test(boundSocket.bindToPort(8080), (string)"Can bind socket to port");
    test(!nonboundSocket.init(), (string)"Cannot reinitialize socket that is bound to a port");
    test(!nonboundSocket.bindToPort(8070), (string)"Cannot bind socket that is bound to a port");
  }

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

  return 0;
}