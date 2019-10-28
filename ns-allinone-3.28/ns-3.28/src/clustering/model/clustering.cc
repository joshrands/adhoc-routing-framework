/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "clustering.h"

namespace ns3 {

void buildWCDSCluster(NodeContainer nodes, vector<Ptr<Socket>> sockets)
{
/*
  cout << "Creating network clusters..." << endl;
  for (uint32_t i = 0; i < nodes.GetN(); i++)
  {
    nodes.Get(i)->wcds.improvementScore = 0;
    nodes.Get(i)->wcds.type = "white"; // initialize all nodes to white
    nodes.Get(i)->wcds.pieceId = nodes.Get(i)->GetId();
    Simulator::Schedule(Seconds(0.000001), &runDistributedWCDS, nodes.Get(i), nodes);
  }
*/
}

void runDistributedWCDS(Ptr<Node> node, NodeContainer nodes)
{
/*
//  cout << "Running WCDS on node " << node->GetId() << endl;
  cout << "Node " << node->GetId() << " state: " << node->wcds.type;
  if (node->wcds.type != "white")
    cout << ". Belongs to piece " << node->wcds.pieceId << endl;
  else
    cout << ". No clusterhead for this node." << endl;

  // list connected nodes
  cout << "Connected to nodes: ";
  int score = 1; // start at 1 for controlling self
  for (uint32_t i = 0; i < node->localRssModels.size(); i++)
  {
    cout << node->localRssModels.at(i).parentId << " ";
    int id = node->localRssModels.at(i).parentId;
 //   cout << id;
    if (id != -1 && nodes.Get(id)->wcds.type == "white")// || (nodes.Get(id)->wcds.type == "gray" && nodes.Get(id)->wcds.pieceId == node->wcds.pieceId))
    {
      score++;
      node->wcds.candidates.push_back(nodes.Get(id));
    }
  }
  cout << endl;

  cout << "With improvement score: " << score << endl;
  node->wcds.improvementScore = score;

  // TODO: Broadcast improvement score to neighbors
  node->wcds.overhead += node->localRssModels.size() * 8; // 8 bytes message with id and score

  // check if this node should become black
  Simulator::Schedule(Seconds(0.1), &checkIfVertex, node, nodes);
*/
}

void checkIfVertex(Ptr<Node> node, NodeContainer nodes)
{
/*
  // check if has the greatest score amongs neighbors (>1)
  if (node->wcds.improvementScore > 1)
  {
    //cout << "Comparing node to neighbors..." << endl;
    bool black = true; // until proven not
    for (uint32_t i = 0; i < node->localRssModels.size(); i++)
    {
      int id = node->localRssModels.at(i).parentId;
      if (id != -1 && (nodes.Get(id)->wcds.improvementScore > node->wcds.improvementScore ||
        ((nodes.Get(id)->wcds.improvementScore == node->wcds.improvementScore) && (node->GetId() > unsigned(id)))))
      {
        black = false;
      }
    }

    if (black)
    {
      // node is *new black!
      cout << "Node " << node->GetId() << " is BLACK." << endl;
      node->wcds.type = "black";
      // make neighbors gray and fix neighbor scores
      for (uint32_t i = 0; i < node->localRssModels.size(); i++)
      {
        int id = node->localRssModels.at(i).parentId;

        if (id == -1)
        {
          node->nextNodeToServer = -1;
          node->connectedToServer = true;
          node->serverPathExists = true;
          node->distanceFromServer = 0;
          continue;
        }

        if (id != -1)
        {
//          nodes.Get(id)->wcds.type = "gray";
          if (nodes.Get(id)->wcds.type == "white")// || nodes.Get(id)->wcds.pieceId == node->wcds.pieceId)
          {
            nodes.Get(id)->wcds.pieceId = node->GetId();
            nodes.Get(id)->wcds.type = "gray";
            //Simulator::Schedule(Seconds(0.000001), &runDistributedWCDS, nodes.Get(id), nodes);

            // find gateway nodes and tell other nodes to send data to CH
            if (node->connectedToServer == false && nodes.Get(id)->serverPathExists)
            {
              // this node has a way, add to gateway node
              node->gatewayNodes.push_back(id);
              if (node->nextNodeToServer != -1 && (nodes.Get(id)->connectedToServer || nodes.Get(id)->distanceFromServer < nodes.Get(node->nextNodeToServer)->distanceFromServer))
              {
                node->nextNodeToServer = id; // this node is now the best path
                node->serverPathExists = true;
                node->distanceFromServer = nodes.Get(id)->distanceFromServer + 1;
              }
            }
            else if (!nodes.Get(id)->serverPathExists)
            {
              nodes.Get(id)->nextNodeToServer = node->GetId();
              nodes.Get(id)->distanceFromServer = node->distanceFromServer + 1;
            }
          }

        }
      }
      node->wcds.pieceId = node->GetId();
      // TODO: Create MIB of monitoring information
      // TODO: Ask children for their monitoring information
    }
  }
  else if (node->wcds.type == "black")
  {
    // get neighbors that don't have a CH
    node->gatewayNodes.clear();
    node->serverPathExists = false;

    vector<int> children;

    for (uint32_t i = 0; i < node->localRssModels.size(); i++)
    {
      int id = node->localRssModels.at(i).parentId;

      if (id == -1)
      {
        node->nextNodeToServer = -1;
        node->serverPathExists = true;
        node->distanceFromServer = 0;
        continue;
      }
//      if (id == -1 || (nodes.Get(id)->wcds.type != "white" && nodes.Get(id)->wcds.pieceId != node->GetId()))
//      {
//        continue;
//      }

      if (id != -1 && nodes.Get(id)->wcds.type == "white")// || nodes.Get(id)->wcds.pieceId == node->wcds.pieceId)
      {
        nodes.Get(id)->wcds.pieceId = node->GetId();
        nodes.Get(id)->wcds.type = "gray";
        //Simulator::Schedule(Seconds(0.000001), &runDistributedWCDS, nodes.Get(id), nodes);
      }

      if (nodes.Get(id)->wcds.pieceId == node->GetId())
        children.push_back(id);

      // find gateway nodes and tell other nodes to send data to CH
//      if (!node->serverPathExists && id != -1 && nodes.Get(id)->serverPathExists)
      if (id != -1 && nodes.Get(id)->serverPathExists)
      {
        // this node has a way, add to gateway node
        node->gatewayNodes.push_back(id);
        // we now have a path!
        node->serverPathExists = true;

        if (node->nextNodeToServer != -1 && (nodes.Get(id)->connectedToServer || nodes.Get(id)->distanceFromServer < nodes.Get(node->nextNodeToServer)->distanceFromServer))
        {
          node->nextNodeToServer = id; // this node is now the best path
          node->distanceFromServer = nodes.Get(id)->distanceFromServer + 1;
        }
      }
      else if (id != -1 && !nodes.Get(id)->serverPathExists)
      {
        nodes.Get(id)->nextNodeToServer = node->GetId();
        nodes.Get(id)->distanceFromServer = node->distanceFromServer + 1;
      }
    }

    // absorb
    for (uint32_t i = 0; i < node->localRssModels.size(); i++)
    {
      int id = node->localRssModels.at(i).parentId;
      if (id == -1)
        continue;

      if (nodes.Get(id)->wcds.type == "black" && unsigned(id) != node->GetId())
      {
        // check if this node is connected to my children, if so get absorbed
        bool absorb = true;
//        cout << "Black connected to " << endl;
        for (uint32_t child = 0; child < children.size(); child++)
        {
          bool found = false;
//          cout << children.at(child) << " = ";
          for (uint32_t j = 0; j < nodes.Get(id)->localRssModels.size(); j++)
          {
//            cout << nodes.Get(id)->localRssModels.at(j).parentId << endl;
            if (signed(nodes.Get(id)->localRssModels.at(j).parentId) == signed(children.at(child)))
            {
//              cout << signed(nodes.Get(id)->localRssModels.at(j).parentId) << " = " << signed(children.at(child)) << endl;
 //             cout << "found" << endl;
              found = true;
            }
          }
          if (!found)
            absorb = false;
        }

        if (absorb)
        {
          cout << node->GetId() << " getting absorbed by " << id << endl;
          for (uint32_t child = 0; child < children.size(); child++)
          {
            nodes.Get(children.at(child))->wcds.pieceId = id;
          }
          node->wcds.pieceId = id;
          node->wcds.type = "gray";
        }
      }
    }

  }
  else if (node->wcds.type == "gray" && !node->serverPathExists)
  {
//    if (!node->serverPathExists)
//    {
    bool pathFound = false;
    // see if we can be a gateway node
    for (uint32_t i = 0; i < node->localRssModels.size(); i++)
    {
      int id = node->localRssModels.at(i).parentId;

      if (id == -1)
      {
        node->nextNodeToServer = -1;
        node->serverPathExists = true;
        node->distanceFromServer = 0;
        pathFound = true;
        continue;
      }

      //cout << id << endl;
      if ((unsigned(id) < nodes.GetN() && signed(id) >= 0) &&
      (node->connectedToServer == false && nodes.Get(id)->serverPathExists && unsigned(node->nextNodeToServer) < nodes.GetN() && nodes.Get(id)->distanceFromServer < nodes.Get(node->nextNodeToServer)->distanceFromServer))
      {
        // found a path!
        node->nextNodeToServer = id;
        node->serverPathExists = true;
        node->distanceFromServer = nodes.Get(id)->distanceFromServer + 1;
        pathFound = true;
      }
    }
 //   }
//    else
//    {
//      node->serverPathExists = nodes.Get(node->nextNodeToServer)->serverPathExists;
//      node->distanceFromServer = nodes.Get(node->nextNodeToServer)->distanceFromServer + 1;
//    }
    if (!pathFound)
      node->serverPathExists = false;
  }

  if (unsigned(node->nextNodeToServer) == node->GetId())
    node->serverPathExists = false;

  Simulator::Schedule(Seconds(1), &runDistributedWCDS, node, nodes);
*/
}

}

