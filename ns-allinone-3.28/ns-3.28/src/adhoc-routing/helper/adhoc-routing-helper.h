/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADHOC_ROUTING_HELPER_H
#define ADHOC_ROUTING_HELPER_H

#include "ns3/adhoc_routing.h"
#include "ns3/defines.h"
#include "ns3/node.h"

namespace ns3 {

/* ... */
// TODO: Create a helper with ns3 aodv and sim rem 
class AdHocRoutingHelper : public AdHocRouting 
{
public:
    // default constructor initializes everything 
    AdHocRoutingHelper(Ptr<Node> node, IP_ADDR nodeIp);

protected:
    Ptr<Node> m_node;

};

}

#endif /* ADHOC_ROUTING_HELPER_H */

