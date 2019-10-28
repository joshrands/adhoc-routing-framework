/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/socket.h"

#include <vector>
#include <iostream>

namespace ns3 {

using namespace std;
using namespace ns3;

class WCDS;

void buildWCDSCluster(NodeContainer nodes, vector<Ptr<Socket>> sockets);

void runDistributedWCDS(Ptr<Node> node, NodeContainer nodes);

void checkIfVertex(Ptr<Node> node, NodeContainer nodes);

}

#endif /* CLUSTERING_H */

