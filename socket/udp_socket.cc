/* Modified version of the following code, which was liscenced under MIT
 * source: https://github.com/ARMmbed/mbed-os
 */
/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "udp_socket.h"
using std::memset;


UDPSocket::UDPSocket() : messages(UDP_QUEUE_SIZE), Socket() {
}

UDPSocket::~UDPSocket(){ 
  close(sockfd); 
}

bool UDPSocket::init(void) { 
  if(UDP_DEBUG){
    printf("[DEBUG]: Initializing udp socket\n");
  }
  bool result = initSocket(SOCK_DGRAM);
  return result;
}

// Server initialization
bool UDPSocket::bindToPort(int port) {
  if(UDP_DEBUG){
    printf("[DEBUG]: Binding udp socket to port %d\n", port);
  }
  if(sockfd < 0){
    if (!initSocket(SOCK_DGRAM))
      return false;
  }

  struct sockaddr_in localHost;
  std::memset(&localHost, 0, sizeof(localHost));

  localHost.sin_family = AF_INET;
  localHost.sin_port = htons(port);
  localHost.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (const struct sockaddr *)&localHost, sizeof(localHost)) < 0) {
    close(sockfd);
    if(UDP_DEBUG){
      printf("[DEBUG]: Unsuccessful binding of udp socket to port %d\n", port);
    }
    return false;
  }
  if(UDP_DEBUG){
    printf("[DEBUG]: Successful binding of udp socket to port %d\n", port);
  }

  return true;
}

bool UDPSocket::joinMulticastGroup(const char *address) {
  struct ip_mreq mreq;

  // Set up group address
  mreq.imr_multiaddr.s_addr = inet_addr(address);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  return setOption(IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
}

bool UDPSocket::setBroadcasting(bool broadcast) {
  int option = (broadcast) ? (1) : (0);
  return setOption(SOL_SOCKET, SO_BROADCAST, &option, sizeof(option));
  if(UDP_DEBUG){
    printf("[DEBUG]: Setting udp socket to broadcast mode\n");
  }
}

// -1 if unsuccessful, else number of bytes written
int UDPSocket::sendTo(Endpoint &remote, const char *packet, int length) {
  if(UDP_DEBUG){
    printf("[DEBUG]: Sending %s to %s via UDP\n", packet, remote.getAddressC());
  }
  if (sockfd < 0) {
    if(UDP_DEBUG){
      printf("[DEBUG]: sockfd is in error state\n");
    }
    return -1;
  }

  int returnVal = sendto(sockfd, packet, length, MSG_CONFIRM,
                (const struct sockaddr *)&remote.remoteHost,
                sizeof(remote.remoteHost));
  if(returnVal < 0){
    int errsv = errno;
    printf("[ERROR] Could not send packet %s to %d\n", packet, remote.getAddressI());
    printf("[ERROR] %d\n", errsv);
  }
  return returnVal;
}

int UDPSocket::sendTo(char *buffer, int length, uint32_t dest, int port) {
  Endpoint remote;
  remote.setAddress(dest, port);
  return sendTo(remote, buffer, length);
}

int UDPSocket::receiveFrom(Endpoint &sender, char *buffer, int length) {
  // -1 if unsuccessful, else number of bytes received
  
  if (sockfd < 0){
    if(UDP_DEBUG){
      printf("[DEBUG]: UDP socketfd is in error state while trying to receive packets\n");
    }
    return -1;
  }
  
  sender.resetAddress();
  socklen_t remoteHostLen = sizeof(sender.remoteHost);
  return recvfrom(sockfd, buffer, length, 0,
                  (struct sockaddr *)&sender.remoteHost, &remoteHostLen);
}

void UDPSocket::receiveFromPortThread() {
  // Continually calls receiveFrom placing the returned messages on the message
  // queue
  while (true) {
    char *buffer = (char *)malloc(MAXLINE * sizeof(char));
    Endpoint sender;
    int n = receiveFrom(sender, buffer, MAXLINE);
    if (n <= 0) {
      continue;
    }
    buffer[n] = '\0';

    // Get received signal strength
    memset(&stats, 0, sizeof(stats)); // clear old data
    memset(&req, 0, sizeof(iwreq));   // clear old data
    sprintf(req.ifr_name, INTERFACE_NAME); // set interface name
    req.u.data.pointer = &stats; // Set pointers
    req.u.data.length = sizeof(iw_statistics);  // Set pointers
    // Pull in data
    if(ioctl(sockfd, SIOCGIWSTATS, &req) == -1){
      fprintf(stderr, "[ioctl]: [ERROR]: threw error (%s) when trying to get TX strength\n", strerror(errno));
    }
    int level = stats.qual.level; //
    
    messages.push(Message(sender, buffer, n, level));
  }
}

bool UDPSocket::getMessage(Message &message) { return messages.pop(message); }

bool UDPSocket::areThereMessages(){ 
  Message temp;
  return messages.peek(temp);
} 

int UDPSocket::getSockfd() const{
  return sockfd;
}