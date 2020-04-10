#include <assert.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <utility>

#include "endpoint.h"
#include "udp_socket.h"
using namespace std;

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string END = "\033[0m\n";

void test(bool condition, string desc)
{
	if (condition)
		cout << GREEN << "[PASS]: " << desc << END;
	else 
		cout << RED << "[FAIL]: " << desc << END;
}

void test(bool condition, const char *desc) {
    if (condition) {
        cout << GREEN << "[PASS]: " << desc << END;
    } else
        cout << RED << "[FAIL]: " << desc << END;
}

bool getYesNo(string message) {
    cout << message << "(y/n): ";
    string response;
    cin >> response;
    for (auto &c : response) {
        c = tolower(c);
    }
    return (response == "y" || response == "yes");
}

void sendMessageToThreadedServerTest(UDPSocket *server, UDPSocket &sender,
                                     Endpoint &serverEnd, string data) {
    // Continue to send data until received
    Message message;
    int count = 0;
    const int MAX_ATTEMPTS = 2;
    do {
        count++;
        sender.sendTo(serverEnd, &data[0], data.length());
        sleep(1);
    } while ((!server->getMessage(message) && count < MAX_ATTEMPTS));
    // Check if test was successful
    char maxAttemptsMessage[60];
    sprintf(&maxAttemptsMessage[0],
            "Can send message to threaded socket with < %d attempts",
            MAX_ATTEMPTS);
    test(count < MAX_ATTEMPTS, &maxAttemptsMessage[0]);
    string messageSentMessage = "Can send '" + data + "' to threaded socket";
    test(strcmp(message.getData(), &data[0]) == 0, messageSentMessage);

    // Clear the servers queue
    do {
        sleep(1);
    } while (server->getMessage(message));
}

void sendMessageBetweenThreadedSockets(UDPSocket *socket1, UDPSocket *socket2,
                                       Endpoint &socket2End, string data1,
                                       string data2) {
    // Send data from 1 to 2
    Message message1to2;
    int count = 0;
    const int MAX_ATTEMPTS = 2;
    do {
        count++;
        socket1->sendTo(socket2End, &data1[0], data1.length());
        sleep(1);
    } while ((!socket2->getMessage(message1to2) && count < MAX_ATTEMPTS));
    // Check if test was successful
    char maxAttemptsMessage[60];
    sprintf(&maxAttemptsMessage[0],
            "Can send message to another socket with < %d attempts",
            MAX_ATTEMPTS);
    test(count < MAX_ATTEMPTS, &maxAttemptsMessage[0]);
    string messageSentMessage = "Socket 1 sent '" + data1 + "' to socket 2";
    test(strcmp(message1to2.getData(), &data1[0]) == 0, messageSentMessage);
    printf("received %s from %s (%s)\n", message1to2.getData(), message1to2.getEndpoint().getAddressC(), message1to2.getAddressC());

    // Send data from 2 to 1
    Message message2to1;
    count = 0;
    do {
        count++;
        socket2->sendTo(message1to2.getEndpoint(), &data2[0], data2.length());
        sleep(1);
    } while ((!socket1->getMessage(message2to1) && count < MAX_ATTEMPTS));
    printf("received %s from %s (%s)\n", message2to1.getData(), message2to1.getEndpoint().getAddressC(), message2to1.getAddressC());

    // Check if test was successful
    sprintf(&maxAttemptsMessage[0],
            "Can send message to another socket with < %d attempts",
            MAX_ATTEMPTS);
    test(count < MAX_ATTEMPTS, &maxAttemptsMessage[0]);
    messageSentMessage = "Socket 2 sent '" + data2 + "' to socket 1";
    test(strcmp(message2to1.getData(), &data2[0]) == 0, messageSentMessage);

    // Clear both sockets
    do {
        sleep(1);
    } while (socket1->getMessage(message2to1) ||
             socket2->getMessage(message1to2));
}

/*!
 * @brief tests wether a socket can broadcast to a group of sockets
 *
 * @param broadcastingSocket
 */
void broadcastMessage(UDPSocket *broadcastingSocket, Endpoint end,
                      string sendData, string expectedResponse) {
    // Send sendData from 1 to 2
    Message messageReceive;
    int count = 0;
    const int MAX_ATTEMPTS = 4;
    do {
        count++;
        broadcastingSocket->sendTo(end, &sendData[0], sendData.length());
        sleep(1);
    } while (!broadcastingSocket->getMessage(messageReceive) &&
             count < MAX_ATTEMPTS);

    // Check if test was successful
    char maxAttemptsMessage[60];
    sprintf(&maxAttemptsMessage[0],
            "Can broadcast to another socket with < %d attempts", MAX_ATTEMPTS);
    test(count < MAX_ATTEMPTS, &maxAttemptsMessage[0]);
    string messageSentMessage = "Broadcasting socket received: '" +
                                expectedResponse +
                                "' in response from python socket";
    test(strcmp(messageReceive.getData(), &expectedResponse[0]) == 0,
         messageSentMessage);

    // Clear both sockets
    do {
        sleep(1);
    } while (broadcastingSocket->getMessage(messageReceive));
}

/*!
 * @brief Create a Threaded Socket object
 *
 * @param returnThread the thread which the socket will be on
 * @param port the port to bind to
 * @return UDPSocket* the socket
 */
UDPSocket *createThreadedSocket(thread &returnThread, int port) {
    UDPSocket *threadedSocket = new UDPSocket();
    if (!threadedSocket->bindToPort(port)) {
        fprintf(stderr, "[ERROR]: Could not bind the receiver socket to port:%d\n",
                port);
    }
    // Thread a recvfrom call
    returnThread = thread(&UDPSocket::receiveFromPortThread, threadedSocket);
    returnThread.detach();
    return threadedSocket;
}

int main() {
    // Test initialization and ports
    {
        UDPSocket nonboundSocket;
        test(nonboundSocket.init(),
             (string) "Can initialize noninitialized socket");
        test(nonboundSocket.bindToPort(8080),
             (string) "Can bind previously initialized socket to a port");
        UDPSocket boundSocket;
        test(boundSocket.bindToPort(8070), (string) "Can bind socket to port");
        test(!nonboundSocket.init(),
             (string) "Cannot initialize socket that is bound to a port");
        test(!nonboundSocket.bindToPort(8070),
             (string) "Will not rebind socket that is already bound to a port");
    }

    printf("________________________________\n\n");

    // Test messages can be sent
    {
        // Create receiving socket
        int port = 8080;
        // Bind to 8080
        thread receiving;
        UDPSocket *receiver = createThreadedSocket(receiving, port);
        // Create sending socket
        UDPSocket sender;
        sender.init();
        Endpoint sendLocation;
        sendLocation.setAddress("127.0.0.1", port);

        // Send a few messages
        sendMessageToThreadedServerTest(receiver, sender, sendLocation,
                                        "hello");
        sendMessageToThreadedServerTest(
            receiver, sender, sendLocation,
            "Weird little people are eating the bakery");
        sendMessageToThreadedServerTest(
            receiver, sender, sendLocation,
            "120.1.7.9:80192 Little Node Buffer Size");
        sendMessageToThreadedServerTest(receiver, sender, sendLocation,
                                        "INFINITE NUMBER HAHAHA");
        receiving.~thread();
    }

    printf("________________________________\n\n");

    // Test communication between sockets
    {
        // Create socket 1
        int port1 = 8881;
        thread socket1ing;
        UDPSocket *socket1 = createThreadedSocket(socket1ing, port1);

        // Create socket 2
        int port2 = 8882;
        thread socket2ing;
        UDPSocket *socket2 = createThreadedSocket(socket2ing, port2);

        // Create endpoint for socket1
        Endpoint socket2End;
        socket2End.setAddress("127.0.0.1", port2);

        // Send a few messages between them
        sendMessageBetweenThreadedSockets(socket1, socket2, socket2End,
                                          "Hello socket 2 how are you today?",
                                          "Doing well socket 1, doing well.");
        sendMessageBetweenThreadedSockets(socket1, socket2, socket2End,
                                          "Play us a song your the piano man",
                                          "Sing us a song tonight");
        sendMessageBetweenThreadedSockets(
            socket1, socket2, socket2End,
            "Well were all in the mood for a melody",
            "And you got us feeling all right");

        // Kill the threads
        socket1ing.~thread();
        socket2ing.~thread();
    }
    printf("________________________________\n\n");

    // Test broadcasting
    {
        system("python3 ./testing_utils/test_broadcasting.py > /dev/null &");
        sleep(2);

        int port1 = 58080;
        thread broadcasting;
        UDPSocket *broadcastSocket = createThreadedSocket(broadcasting, port1);
        if (!broadcastSocket->setBroadcasting()) {
            fprintf(stderr, "Could not set port to broadcasting\n");
            exit(-1);
        }

        int port2 = 58083;
        Endpoint end;
        end.setAddress("255.255.255.255", port2);
        broadcastMessage(broadcastSocket, end, "Hello World!",
                         "Hello Broadcaster!");
    }
    printf("________________________________\n\n");

    return 0;
}