// Client side implementation of UDP client-server model
#include "Endpoint.h"
#include "ServerSocket.h"
#include "Socket.h"
#include "UDPSocket.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

// Driver code
int main() {
  char *ECHO_SERVER_ADDRESS = "127.0.0.1";
  UDPSocket client;
  if (!client.init()) {
    fprintf(stderr, "Couldn't initialize client\n");
    exit(-1);
  }
  Endpoint server;
  if (!server.setAddress(ECHO_SERVER_ADDRESS, PORT)) {
    fprintf(stderr, "Couldn't set server address\n");
    exit(-1);
  }

  char *buffer = "Hello world";
  int code = client.sendTo(server, buffer, strlen(buffer));
  if (code <= 0) {
    fprintf(stderr, "Couldn't send any information to the server\n");
    exit(-1);
  }
  return 0;
}