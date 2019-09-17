// Server side implementation of UDP socket
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
 
#define MAXLINE 1024 


class ServerSocket{
private:
    int sockfd;
    struct sockaddr_in serverAddress;
public:
    // TODO: Look at INADDR_ANY for other options and usage
    // http://man7.org/linux/man-pages/man7/ip.7.html
    ServerSocket(int port){
        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
        memset(&serverAddress, 0, sizeof(serverAddress));

        // Filling server information
        serverAddress.sin_family = AF_INET; // IPv4
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        // Bind the socket with the server address
        if ( bind(sockfd, (const struct sockaddr *)&serverAddress,
                  sizeof(serverAddress)) < 0 )
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    }

    ~ServerSocket(){
        close(sockfd);
    }

    /*!
     * Listens for messages sent to the server's port
     * @return char buffer with delimiting \0
     */
    // TODO: Look at MSG_WAITALL options
    char* listen() {
        // Create temporary client socket address
        struct sockaddr_in cliaddr;
        memset(&cliaddr, 0, sizeof(cliaddr));
        return listen(cliaddr);
    }
    /*!
     * Listens for messages sent to the server's port
     *
     * @param cliaddr will modify this variable and return the socket's
     * address that sent the message
     * @return char buffer with delimiting \0
     */
    char* listen(struct sockaddr_in& cliaddr){
        socklen_t len;
        int n;
        char *buffer = (char *) malloc(sizeof(char) * MAXLINE);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                     &len);
        buffer[n] = '\0';
        return buffer;
    }

    // TODO: Look at MSG_DONTROUTE
    // TODO: or other sendto() flags
    /*!
     * Will send a buffer with delimiting \0 to the client
     * @param cliaddr the address of the client
     * @param buffer the buffer to send
     */
    void sendTo(struct sockaddr& cliaddr, char buffer[MAXLINE]){
        sendto(sockfd, (const char *) buffer, strlen(buffer),
               MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
               sizeof(cliaddr));
    }
    // TODO: Look at sendmsg and send which I believe is for multicast
    // http://man7.org/linux/man-pages/man2/send.2.html
};
