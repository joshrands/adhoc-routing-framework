/*********************************
 * network_monitor.h
 *
 * Abstract implementation of network monitoring 
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

class NetworkMonitor
{
public:
    NetworkMonitor();
    NetworkMonitor(int nodeId);

    virtual void initialize(int parentId) = 0;
    virtual void initializeBatteryModel() = 0;
    virtual void initializeRssModel(int pairId) = 0;

    virtual double getBatteryLevel() = 0;
    virtual double getRSSBetweenNodes(int pairId) = 0;

    int getParentId() { return this->parentId; }

protected:
    // node being monitored 
    int parentId;
};
