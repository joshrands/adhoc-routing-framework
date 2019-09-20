// Client side implementation of UDP client-server model 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
 
#define MAXLINE 1024 

class ClientSocket{
private:
    int sockfd;
    struct sockaddr_in servaddr;
public:
    /*!
     * Will create a UDP client socket
     *
     * @param port the port to send to
     */
    ClientSocket(int port){
        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));

        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        servaddr.sin_addr.s_addr = INADDR_ANY;
    }

    /*!
     * Creates a UDP client socket
     * @param port the port to send messages to
     * @param ipAddress the ipAddress the server is on
     */
    ClientSocket(int port, const char* ipAddress){
        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));

        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        servaddr.sin_addr.s_addr = inet_addr(ipAddress);
        printf(ipAddress);
    }

    ~ClientSocket(){
        close(sockfd);
    }

    /*!
     * Will send a buffer with delimiting \0 to the server
     * @param buffer the buffer to send
     */
    // TODO: Look at MSG_DONTROUTE
    // TODO: or other sendto() flags
    void sendTo(char* buffer){
        sendto(sockfd, (const char *)buffer, strlen(buffer),
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        printf(buffer);
    }
    // TODO: Look at sendmsg and send which I believe is for multicast
    // http://man7.org/linux/man-pages/man2/send.2.html
};
