#ifndef SA_H
#define SA_H

#include "RandomGenerator.hpp"
#include "State.hpp"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      int init(char* argv[]);
      void run();
      void printSummary();
      void initTable();

   private:
      //constant
      int MAX_STATE_CHANGE_PER_TEMP;
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

      void printState(const int& trial, const int& worstAcc);
};

#endif
