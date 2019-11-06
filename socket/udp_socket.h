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

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "endpoint.h"
#include "socket.h"
#include "message.h"
#include "safe_circular_queue.h"
#include "socket_defines.h"
#include <iostream>
#include <utility>

using namespace std;

/*!
UDP Socket
*/
class UDPSocket : public Socket {

public:
  /*! Instantiate an UDP Socket
   */
  UDPSocket();

  ~UDPSocket() { close(sockfd); }

  /*! Init the UDP Client Socket without binding it to any specific por
   *  @return true on success, false on failure
   */
  bool init(void);

  /*! Bind a UDP Server Socket to a specific port
   *  @param port The port to listen for incoming connections on
   *  @return true on success, false on failure
   */
  bool bindToPort(int port);

  /*! Join the multicast group at the given address
   *  @param address The address of the multicast group
   *  @return true on success, false on failure
   */
  bool joinMulticastGroup(const char *address);

  /*! Set the socket in broadcasting mode
   *  @return true on success, false on failure
   */
  bool setBroadcasting(bool broadcast = true);

  /*! Send a packet to a remote endpoint
   *  @param remote The remote endpoint
   *  @param packet The packet to be sent
   *  @param length The length of the packet to be sent
   *  @return the number of written bytes on success (>=0) or -1 on failure
   */
  int _sendTo(Endpoint &remote, const char *packet, int length);

  /*!
   * Send a packet to an IP address and port
   * 
   * @param buffer 
   * @param length 
   * @param dest 
   * @return int 
   */
  int sendTo(char* buffer, int length, uint32_t dest, int port);

  /*! Receive a packet from a remote endpoint
   *  @param remote The remote endpoint
   *  @param buffer The buffer for storing the incoming packet data. If a packet
   *                is too long to fit in the supplied buffer, excess bytes are discarded
   *  @param length The length of the buffer
   *  @return the number of received bytes on success (>=0) or -1 on failure
   */
  int receiveFrom(Endpoint &remote, char *buffer, int length);

  /*! Continuously reads data from the port, placing messages onto the
   *  socket's message queue.
   */
  void receiveFromPortThread();

  /*!
   * @brief Get one message from the socket
   * 
   * @param message the first message on the queue
   * @return true a message was received
   * @return false no messages
   */
  bool getMessage(Message &message);

  /*!
   * @brief Check if there are any messages without attempting to get the packet
   * 
   */
  bool areThereMessages();

  /*!
   * @brief Get the sockfd object
   * 
   * @return int 
   */
  int getSockfd() const;

private:
  // To hold threaded messages
  SafeCircularQueue<Message> messages;
};

#endif