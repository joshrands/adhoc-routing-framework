/* Highly modified version of the following code, which was liscenced under MIT
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
#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 1024

using namespace std;
// TimeInterval class prototype
class TimeInterval;

class Socket {
public:
  Socket();

  ~Socket() { close(sockfd); }

  /*! Set blocking or non-blocking mode of the socket and a timeout on
   *  blocking socket operations
   *  @param blocking true for blocking mode, false for non-blocking mode
   *  @param param timeout timeout in ms [Default: (1500)ms]
   */
  void setBlocking(bool blocking, unsigned int timeout = 1500);

  /*! Set socket options
   *  @param level stack level
   *  @param optname option ID
   *  @param optval option value
   *  @param socklen_t length of the option value
   *  @return true on success, false on failure
   */
  bool setOption(int level, int optname, const void *optval, socklen_t optlen);

  /*! Get socket options
   *  @param level stack level
   *  @param optname option ID
   *  @param optval buffer pointer where to write the option value
   *  @param socklen_t length of the option value
   *  @return true on success, false on failure
   */
  bool getOption(int level, int optname, void *optval, socklen_t *optlen);

  /*! Close the socket
   *  @param shutdown free the left-over data in message queues
   *
   */
  int close(bool shutdown = true);

protected:
  int sockfd;
  bool initSocket(int type);

  int waitReadable(TimeInterval &timeout);
  int waitWritable(TimeInterval &timeout);

  bool blocking;
  unsigned int timeout;

private:
  int _select(struct timeval *timeout, bool read, bool write);
};

/*! Time interval class used to specify timeouts
 */
class TimeInterval {
  friend class Socket;

public:
  TimeInterval(unsigned int ms);

private:
  struct timeval time;
};

#endif