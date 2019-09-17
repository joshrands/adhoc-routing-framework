// Client side implementation of UDP client-server model 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ClientSocket.h"
#include "ServerSocket.h"

#define PORT 8080 

// Driver code 
int main() {
	ClientSocket client(PORT, "127.0.0.1");
	char *hello = "Hello from client";
	client.sendTo(hello);
	return 0;
}

