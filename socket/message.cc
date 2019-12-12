#include "message.h"

Message::Message(Endpoint end, char* data, int length, int bitrate){
    this->end = end;
    this->data = data;
    this->length = length;
    this->bitrate = bitrate;
}

uint32_t  Message::getAddressI(void) const{
    return end.getAddressI();
}


char* Message::getAddressC(void){
    return end.getAddressC();
}


int Message::getLength(void) const{
    return length;
}


char* Message::getData(void){
    return data;
}


int Message::getPort(void) const{
    return end.getPort();
}


Endpoint& Message::getEndpoint(void){
    return end;
}

int Message::getBitrate(void) const{
    return bitrate;
}