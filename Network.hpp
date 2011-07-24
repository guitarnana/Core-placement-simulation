#pragma once

#include "Router.hpp"
#include "Core.hpp"

class Network{
   public:
      Network();
      Network(const Network& sourceNetwork);
      Network& operator=(const Network& sourceNetwork); 
      ~Network();

      void init(int r, int c);
      int getRow();
      int getCol();
      void addCore(Coordinate pos);
      void removeCore(Coordinate pos);
      bool hasCore(Coordinate pos); //check if that router has a core or not
      
   private:
      int row;
      int col;
      Router **routers;
};
