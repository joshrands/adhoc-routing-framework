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

void test(bool condition, string message) {
  cout << message << ": ";
  assert(condition);
  cout << "PASS" << endl;
}

int main() {
  // Test messages can be sent
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
  // Create sending socket
  UDPSocket sender;
  sender.init();
  Endpoint sendLocation;
  sendLocation.setAddress("127.0.0.1", port);
  char testMessage[6] = "Hello";
  // Continue to send data until received
  pair<Endpoint, char *> message;
  int count = 0;
  const int MAX_ATTEMPTS = 2;
  do {
    count++;
    sender.sendTo(sendLocation, testMessage, sizeof(testMessage));
    sleep(1);
  } while ((!receiver->getMessage(message) && count < MAX_ATTEMPTS));
  test(count < MAX_ATTEMPTS, "Can send simple message to threaded socket with < 2 attempts");
  test(strcmp(message.second, testMessage) == 0, "Can send 'hello' to threaded socket");
  while (receiver->getMessage(message)) {
    sleep(1);
  }

  // Send another message
  char testMessage1[42] = "Weird little people are eating the bakery";
  count = 0;
  do {
    count++;
    sender.sendTo(sendLocation, testMessage1, sizeof(testMessage1));
    sleep(1);
  } while (!receiver->getMessage(message) && count < MAX_ATTEMPTS);
  test(count < MAX_ATTEMPTS, "Can send longer message to threaded socket with < 3 attempts");
  test(strcmp(message.second, testMessage1) == 0,
       "Can send 'Weird little people are eating the bakery' to threaded socket");

  receiving.detach();
  return 0;
}