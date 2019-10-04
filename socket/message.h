#ifndef MESSSAGE_H
#define MESSAGE_H

#include "endpoint.h"

class Message{
public:
    Message(){};
    Message(Endpoint end, char* data, int length);

    /*!
     * @brief Returns the IP address of the message sender
     * 
     * @return uint32_t 
     */
    uint32_t getAddressI(void) const;

    /*!
     * @brief Returns the IP address of the message sender
     * 
     * @return char array
     */
    char *getAddressC(void);

    /*!
     * @brief Get the Length of the message
     * 
     * @return int 
     */
    int getLength(void) const;

    /*!
     * @brief Get the Data of the message
     * 
     * @return char* 
     */
    char* getData(void);

    /*!
     * @brief Get the Port object
     * 
     * @return int 
     */
    int getPort(void);

    /*!
     * @brief Get the Endpoint object
     * 
     * @return Endpoint 
     */
    Endpoint& getEndpoint(void);

private:
    Endpoint end;
    char* data;
    int length;
};

#endif