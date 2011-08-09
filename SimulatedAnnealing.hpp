#pragma once

#include "RandomGenerator.hpp"
#include "State.hpp"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      int init(char* filename);
      void run();
      void printState();

   private:
      //constant
      int TEMP_STEP;
      int MAX_STATE_CHANGE_PER_TEMP;
      int SUCCESS_PER_TEMP;
      double TEMP_CHANGE_FACTOR;
      double END_TEMP;

      //variable
      State currentState;
      State bestState;
      RandomGenerator random;
      double temp;

      //function
      int getCost();
      bool acceptChange(int cost);
      bool isAccept(double value);
};
