#include "endpoint.h"
#include "udp_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
using namespace std;

int main() {
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
    }
  }
  receiving.join();
  return 0;
}