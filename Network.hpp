#pragma once

#include <vector>

#include "Router.hpp"
#include "Core.hpp"

using std::vector;

#define NO_DIR -1
#define TOP    0
#define BOTTOM 1
#define LEFT   2
#define RIGHT  3

#define MAX_DIRECTION 4

#define NO_NODE -1

struct Link {
   int toNodeId; //link to what nodeId
   int connection; //number of connection in using this link
   int bandwidth; //bandwidth going through this link

   Link() { //constructor
      toNodeId = NO_NODE;
      connection = 0;
      bandwidth = 0;
   }
};

class Network{
   public:
      Network();
      Network(const Network& sourceNetwork);
      Network& operator=(const Network& sourceNetwork); 
      ~Network();

      void init(int r, int c);
      int getRow();
      int getCol();
      void addCore(Coordinate pos, int coreIndex);
      void removeCore(Coordinate pos);
      int getCoreIndex(Coordinate pos); 
      bool hasCore(Coordinate pos);//check if position has a core or not
      void addConnection(Coordinate from, Coordinate to);
      void removeConnection(Coordinate from, Coordinate to);
      void removeAllConnections(int** bandwidth, vector<Core> core, int index);
      void addAllConnections(int** bandwidth, vector<Core> core, int index); 

      bool isLegal(int LINK_BANDWIDTH);

      void updateNetwork(int** bandwidth, vector<Core> core);
      double calculateUtilization();

      void printNetwork();
      void printUtil();
      
   private:
      int row;
      int col;
      bool legal;
      Router **routers;
      Link **utilization;

      void deepCopy(const Network& sourceNetwork);
      int getDirection(Coordinate from, Coordinate to);
};
