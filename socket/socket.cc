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

#include "socket.h"
#include <cstring>

using std::memset;

Socket::Socket() : sockfd(-1) {}

Socket::~Socket() { sclose(); }

bool Socket::initSocket(int type) {
  if (sockfd != -1) {
    perror("[ERROR]: Socket already created\n");
    return false;
  }

  int fd = socket(AF_INET, type, 0);
  if (fd < 0) {
    perror("[ERROR]: Socket creation failed\n");
    return false;
  }
  sockfd = fd;

  return true;
}

bool Socket::setOption(int level, int optname, const void *optval, socklen_t optlen) {
  return (setsockopt(sockfd, level, optname, optval, optlen) < 0) ? false : true;
}

bool Socket::getOption(int level, int optname, void *optval, socklen_t *optlen) {
  return (getsockopt(sockfd, level, optname, optval, optlen) < 0) ? false : true;
}
void Socket::sclose() { close(sockfd); }

int Socket::getTransmissionPower(){
  // Structs for quering transmission power
  struct iw_statistics stats;
  struct iwreq req;
  // Get transmission power
  memset(&stats, 0, sizeof(stats)); // clear old data
  memset(&req, 0, sizeof(iwreq));   // clear old data
  sprintf(req.ifr_name, INTERFACE_NAME); // set interface name
  req.u.data.pointer = &stats; // Set pointers
  req.u.data.length = sizeof(iw_statistics);  // Set pointers
  // Pull in data
  if(ioctl(sockfd, SIOCGIWTXPOW, &req) == -1){
    fprintf(stderr, "[ioctl]: [ERROR]: threw error (%s) when trying to get TX strength\n", strerror(errno));
    return -1;
  }
  return req.u.txpower.value;   
}

bool Socket::setTransmissionPower(int txPwr){
  // Structs for setting transmission power
  struct iw_statistics stats;
  struct iwreq req;
  // Set transmission signal strength
  memset(&stats, 0, sizeof(stats)); // clear old data
  memset(&req, 0, sizeof(iwreq));   // clear old data
  sprintf(req.ifr_name, INTERFACE_NAME); // set interface name
  req.u.data.pointer = &stats; // Set pointers
  req.u.data.length = sizeof(iw_statistics);  // Set pointers
  req.u.txpower.value = txPwr;
  // Pull in data
  if(ioctl(sockfd, SIOCSIWTXPOW, &req) == -1){
    fprintf(stderr, "[ioctl]: [ERROR]: threw error (%s) when trying to set TX strength\n", strerror(errno));
    return false;
  }
  // Check change was performed   
  return getTransmissionPower() == txPwr;
}
