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
#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class UDPSocket;

/**
IP Endpoint (address, port)
*/
class Endpoint {
  friend class UDPSocket;

public:
  Endpoint(void);

  ~Endpoint(void);

  /*! Reset the address of this point
   */
  void resetAddress(void);

  /*! Set the address of this endpoint
   *  @param host The endpoint IP address
   *  @param port The endpoint port
   *  @return true on success or false on failure
   */
  bool setAddress(const char *host, const int port);

  /*! Set the address of this endpoint
   *  @param host The endpoint IP address
   *  @param port The endpoint port
   *  @return true on success or false on failure
   */
  bool setAddress(const uint32_t host, const int port);

  /*! Get the IP address of this endpoint
   *  @return the IP address of this point
   */
  char *getAddressC(void);

  /*! Get the IP address of this endpoint
   *  @return the integer IP address of this point
   */
  uint32_t getAddressI(void) const;

  /*! Get the port of this endpoint
   *  @return the port of this point
   */
  int getPort(void) const;

  bool operator==(Endpoint &rhs);
  bool operator!=(Endpoint &rhs);

protected:
  char ipAddress[17];
  struct sockaddr_in remoteHost;
};

#endif
