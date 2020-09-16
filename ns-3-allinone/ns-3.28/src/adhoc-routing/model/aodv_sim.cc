#include "aodv_sim.h"

int SimAODV::handlePackets()
{
	int count = packetQueue.size();
	while(!packetQueue.empty()){
        // Get the packet
		SimPacket p = packetQueue.front();
		// Pop the packet off the queue
		packetQueue.pop();
        // update pair data from packet

        // IMPORTANT: We are not updating pair data because it is not accurate
        // enough for link breakage determination. We are using hello messages
        // to determine neighbors and storing last known rss with those hello
        // messages. No RSS prediction or REM models are done/calculated. 
        networkMonitor->updatePairData(p.packetPairData);

        // handle this packet
		_handlePacket(p.portId, p.data, p.length, p.source);
	}
	return count;
} 

// Private Functions

bool SimAODV::_socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest)
{
    return simSocketSendPacket(buffer, length, dest, portId, this->getIp());
} 

void SimAODV::_buildPort(Port*)
{
    // unnecessary in simulation
} 

void SimAODV::_destroyPort(Port*)
{
    // unnecessary in simulation
} 

double SimAODV::getBandwidthShare(IP_ADDR linkIp) {
	double retVal = 0.0;

	if(linkExists(linkIp))
	{
		if(networkMonitor != nullptr)
		{
			vector<IP_ADDR> reighborsList = this->getCopyOfNeighbors();
			double runningTotal = 0;

			for(IP_ADDR nbIp: reighborsList)
			{
				runningTotal += networkMonitor->getRSSBetweenNodes(nbIp, this->ipAddress);
			}

			retVal = networkMonitor->getRSSBetweenNodes(linkIp, this->ipAddress)/runningTotal;
		}
	}

	if(DEBUG)
	{
	    std::cout << "[DEBUG][AODV_SIM]: " << getStringFromIp(linkIp) << " to " << getStringFromIp(this->ipAddress) <<
	        " has link share of " << retVal << std::endl;
	}

    return retVal;
}

/* OLD 
AODVSim::AODVSim()
{

}

AODVSim::AODVSim(IP_ADDR ip) : AODV(ip)
{
    cout << "[INFO]: Creating new simulated aodv" << endl;
}

void AODVSim::getOneHopNeighbors()
{
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Deprecated? Grabbing one hop neighbors from network monitoring" << endl;
}

int AODVSim::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port)
{
    // call other function
    return simSocketSendPacket(buffer, length, dest, port, this->getIp());
}
*/
