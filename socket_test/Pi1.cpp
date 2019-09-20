// Client side implementation of UDP client-server model
#include "Endpoint.h"
#include "ServerSocket.h"
#include "Socket.h"
#include "UDPSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

// Driver code
int main() {
  UDPSocket server;
  if (!server.bindToPort(PORT)) {
    fprintf(stderr, "Couldn't bind to port\n");
    exit(-1);
  }
  while (true) {
    char buffer[MAXLINE];
    Endpoint client;
    int n = server.receiveFrom(client, buffer, MAXLINE);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);
  }
  return 0;
}