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
#include <cstring>
#include <errno.h>
using std::memset;

UDPSocket::UDPSocket() : messages() {}

bool UDPSocket::init(void){ 
  printf("[DEBUG]: Initializing udp socket\n");
  return initSocket(SOCK_DGRAM); 
}

// Server initialization
bool UDPSocket::bindToPort(int port) {
  printf("[DEBUG]: Binding udp socket to port %d\n", port);
  if(getSockfd() < 0){
    if (!initSocket(SOCK_DGRAM))
      return false;
  }

  struct sockaddr_in localHost;
  std::memset(&localHost, 0, sizeof(localHost));

  localHost.sin_family = AF_INET;
  localHost.sin_port = htons(port);
  localHost.sin_addr.s_addr = INADDR_ANY;

  if (bind(getSockfd(), (const struct sockaddr *)&localHost, sizeof(localHost)) < 0) {
    sclose();
    printf("[DEBUG]: Unsuccessful binding of udp socket to port %d\n", port);
    return false;
  }
  printf("[DEBUG]: Successful binding of udp socket to port %d\n", port);
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
int UDPSocket::sendTo(Endpoint &remote, char *packet, int length) {
  if(UDP_DEBUG){
    printf("[DEBUG]: Sending %s to %s via UDP\n", packet, remote.getAddressC());
  }

  int returnVal = sendto(getSockfd(), packet, length, MSG_CONFIRM,
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
  if(UDP_DEBUG){
    printf("[DEBUG]: Sending %s to %d via UDP\n", buffer, dest) ;
  }

  in_addr dest_in;
  dest_in.s_addr = dest;
  sockaddr_in remote;
  remote.sin_family = AF_INET;
  remote.sin_addr = dest_in;
  remote.sin_port = port;
  int returnVal = sendto(getSockfd(), buffer, length, MSG_CONFIRM,
                (const struct sockaddr *)&remote,
                sizeof(remote));
  if(returnVal < 0){
    int errsv = errno;
    printf("[ERROR] Could not send packet %s to %d\n", buffer, dest);
    printf("[ERROR] %d\n", errsv);
  }
  return returnVal;
}

int UDPSocket::receiveFrom(Endpoint &remote, char *buffer, int length) {
  // -1 if unsuccessful, else number of bytes received
  remote.resetAddress();
  socklen_t remoteHostLen = sizeof(remote.remoteHost);
  return recvfrom(getSockfd(), buffer, length, 0,
                  (struct sockaddr *)&remote.remoteHost, &remoteHostLen);
}

void UDPSocket::receiveFromPortThread() {
  // Continually calls receiveFrom placing the returned messages on the message
  // queue
  while (true) {
    char *buffer = (char *)malloc(MAXLINE * sizeof(char));
    Endpoint client;
    socklen_t remoteHostLen = sizeof(client.remoteHost);
    int n = recvfrom(getSockfd(), buffer, MAXLINE, 0,
                  (struct sockaddr *)&client.remoteHost, &remoteHostLen);
    
    if (n <= 0) {
      continue;
    }

    buffer[n] = '\0';
    messages.push(Message(client, buffer, n));
    // Need to find out why this is neccesary
    
  }
}

bool UDPSocket::getMessage(Message &message) { return messages.pop(message); }

bool UDPSocket::areThereMessages(){ 
  Message temp;
  return messages.peek(temp);
} 