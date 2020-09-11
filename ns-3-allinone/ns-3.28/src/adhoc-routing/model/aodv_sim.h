#ifndef AODVSIM_H
#define AODVSIM_H

/*********************************
 * aodv_sim.h
 *
 * Simulated aodv class implementations 
 * 
 * Author: Josh Rands
 * Date: 9/30/2019
 ********************************/

#include "aodv.h"
#include "network_monitor.h"

struct SimPacket{
    char* data;
    int length;
    int portId;
    IP_ADDR source;
    pair_data packetPairData;
};

class SimAODV : public AODV
{
public:
    // Default constructor 
    SimAODV() : AODV() {}

		/*!
     * @brief Construct a new SimAODV object
     * @param ip the ip address of the current device
     */
    SimAODV(uint32_t ip) : AODV(ip) {}
    SimAODV(const char* ip) : AODV(ip) {}

    // Destructors
    ~SimAODV() {}

    // when handlePackets is called, network monitor gets pair data from the QueuedPacket
    int handlePackets() override;

    // queue for storing packets
    queue<SimPacket> packetQueue;
    // network monitoring
    NetworkMonitor* networkMonitor = nullptr;
	/**
     * @brief Returns the percentage of the bandwidth that can be used by the node linkIp, assuming that all
     * other neighbors attempt to communicate at the same time.
     *
     * @param linkIp, the IP address of the link to check
     */
    double getBandwidthShare(IP_ADDR linkIp);

    // Override this function with a function to send data between two nodes in the desired simulation
    function<int(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)> simSocketSendPacket;

protected:
    bool _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) override;
    void _buildPort(Port*) override;
    void _destroyPort(Port*) override;

};

#endif
