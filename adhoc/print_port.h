#include "port.h"

class PrintPort : public Port{
public:
    PrintPort(int portId):Port(portId){}
    ~PrintPort(){}
    void handlePacket(char* data, int length, IP_ADDR source){
        printf("[PRINT PORT]: Received ");
        for(int i = 0; i < length; i++){
            printf("%c", data[i]);
        }
        printf(" from %s\n", getStringFromIp(source).c_str());
    }

};