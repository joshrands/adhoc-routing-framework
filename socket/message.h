#ifndef MESSSAGE_H
#define MESSAGE_H

#include "endpoint.h"

class Message{
public:
    Message(){};
    Message(Endpoint end, char* data, int length){
        this->end = end;
        this->data = data;
        this->length = length;
    }

    /*!
     * @brief Returns the IP address of the message sender
     * 
     * @return uint32_t 
     */
    uint32_t getIPAddress(void){
        return end.getIntAddress();
    }

    /*!
     * @brief Get the Length of the message
     * 
     * @return int 
     */
    int getLength(void) const{
        return length;
    }

    /*!
     * @brief Get the Data of the message
     * 
     * @return char* 
     */
    char* getData(void){
        return data;
    }

    /*!
     * @brief Get the Port object
     * 
     * @return int 
     */
    int getPort(void){
        return end.getPort();
    }

    /*!
     * @brief Get the Endpoint object
     * 
     * @return Endpoint 
     */
    Endpoint& getEndpoint(void){
        return end;
    }

private:
    Endpoint end;
    char* data;
    int length;
};

#endif