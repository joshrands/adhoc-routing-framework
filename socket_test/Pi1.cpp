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
	ServerSocket server(PORT);
	while(true){
		char* buffer = server.listen();
		printf("Received: %s\n", buffer);
	}
	return 0;
}

