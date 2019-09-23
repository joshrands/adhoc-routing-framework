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

Socket::Socket() : sockfd(-1), blocking(true), timeout(1500) {}

Socket::~Socket() { sclose(); }

void Socket::setBlocking(bool blocking, unsigned int timeout) {
  blocking = blocking;
  timeout = timeout;
}

bool Socket::initSocket(int type) {
  if (sockfd != -1) {
    perror("Socket already created\n");
    return false;
  }

  int fd = socket(AF_INET, type, 0);
  if (fd < 0) {
    perror("socket creation failed\n");
    return false;
  }
  sockfd = fd;

  int truth = 1;
  if (!setOption(SOL_SOCKET, SO_REUSEADDR, &truth, sizeof(int))) {
    fprintf(stderr, "socket not set to reusable\n");
    return false;
  }

  return true;
}

bool Socket::setOption(int level, int optname, const void *optval, socklen_t optlen) {
  return (setsockopt(sockfd, level, optname, optval, optlen) < 0) ? false : true;
}

bool Socket::getOption(int level, int optname, void *optval, socklen_t *optlen) {
  return (getsockopt(sockfd, level, optname, optval, optlen) < 0) ? false : true;
}

int Socket::_select(struct timeval *timeout, bool read, bool write) {
  fd_set fdSet;
  FD_ZERO(&fdSet);
  FD_SET(sockfd, &fdSet);

  fd_set *readset = (read) ? (&fdSet) : (NULL);
  fd_set *writeset = (write) ? (&fdSet) : (NULL);

  int ret = select(FD_SETSIZE, readset, writeset, NULL, timeout);
  return (ret <= 0 || !FD_ISSET(sockfd, &fdSet)) ? (false) : (true);
}

void Socket::sclose() { close(sockfd); }

int Socket::waitReadable(TimeInterval &timeout) { return _select(&timeout.time, true, false); }

int Socket::waitWritable(TimeInterval &timeout) { return _select(&timeout.time, false, true); }

TimeInterval::TimeInterval(unsigned int ms) {
  time.tv_sec = ms / 1000;
  time.tv_usec = (ms - (time.tv_sec * 1000)) * 1000;
}