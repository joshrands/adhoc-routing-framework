#include "aodv.h"

struct QueuedPacket{
    char* data;
    int length;
    int portId;
    IP_ADDR source;
};

/* AODVTest class
 * For testing AODV with unit tests. */
class AODVTest : public AODV
{
public: 	
	// debugging values 
	static int globalPacketCount;
	static IP_ADDR lastNode; 
	static IP_ADDR lastReceive;

	AODVTest(IP_ADDR ip) : AODV(ip) {}
	AODVTest(const char* ip) : AODV(ip) {}
	~AODVTest() { } 

    int handlePackets() override;

	// Network Monitoring 

	// add/remove node to neighbor list
	void addNeighbor(AODVTest* node);
	void removeNeighbor(AODVTest* node);
    void addPhysicalNeighborOnly(AODVTest* node);

	// return true if node is neighbor
	bool isNeighbor(AODVTest* node);

	// see if packet was put in queue
	bool packetInRreqBuffer(IP_ADDR dest);

    // Virtual place to store incoming packets
    queue<QueuedPacket> packetQueue;

    RREQHelper* getRREQHelper(){
        return &rreqHelper;
    }

protected:
    bool _socketSendPacket(int port, char *buffer, int length, IP_ADDR dest) override;
	void _buildPort(Port*) override;
    void _destroyPort(Port*) override;

private:
	vector<AODVTest*> m_physicalNeighbors;

};

class AODVMonitorTest : public AODVTest 
{
protected:
	bool _socketSendPacket(int port, char *buffer, int length, IP_ADDR dest) override;

};

class PrintPort : public Port{
public:
    PrintPort(int portId):Port(portId){}
    ~PrintPort(){}
    void handlePacket(char* data, int length, IP_ADDR source){
        printf("[PRINT PORT]: Received ");
        for(int i = 0; i < length; i++){
            printf("%c", data[i]);
        }
        printf(" from %s\n", getStringFromIp(source));
    }

};