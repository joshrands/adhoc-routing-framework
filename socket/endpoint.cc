/* Modified version of the following code, which was liscenced under MIT
 * source: https://github.com/ARMmbed/mbed-os
 */
/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "endpoint.h"
#include "socket.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>

Endpoint::Endpoint() { resetAddress(); }
Endpoint::~Endpoint() {}

void Endpoint::resetAddress(void) {
  std::memset(&remoteHost, 0, sizeof(struct sockaddr_in));
  ipAddress[0] = '\0';
}

#include "stdio.h"

bool Endpoint::setAddress(const char *host, const int port) {
  resetAddress();
  *ipAddress = *host;
  remoteHost.sin_addr.s_addr = inet_addr(host);

  // Address family
  remoteHost.sin_family = AF_INET;

  // Set port
  remoteHost.sin_port = htons(port);

  return true;
}

bool Endpoint::setAddress(const uint32_t host, const int port){
  resetAddress();
  // Set ip address
  in_addr host_in;
  host_in.s_addr = host;
  *ipAddress = *inet_ntoa(host_in);
  remoteHost.sin_addr.s_addr = host;

  // Address family
  remoteHost.sin_family = AF_INET;

  // Set port
  remoteHost.sin_port = htons(port);

  return true;
}

char *Endpoint::getAddressC() {
  if ((ipAddress[0] == '\0') && (remoteHost.sin_addr.s_addr != 0))
    return inet_ntoa(remoteHost.sin_addr);
  return ipAddress;
}

uint32_t Endpoint::getAddressI(void) const{
  if ((ipAddress[0] == '\0') && (remoteHost.sin_addr.s_addr != 0))
    return remoteHost.sin_addr.s_addr;
  return inet_addr(ipAddress);
}

int Endpoint::getPort() const { return ntohs(remoteHost.sin_port); }

bool Endpoint::operator==(Endpoint &rhs) {
  return (strcmp(this->getAddressC(), rhs.getAddressC()) == 0 && this->getPort() == rhs.getPort());
}

bool Endpoint::operator!=(Endpoint &rhs) { return (!((*this) == rhs)); }
