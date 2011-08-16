#ifndef SA_HPP
#define SA_HPP

#include "State.hpp"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      int init(double alpha, double beta, double gamma, double delta, \
               double startTemp, double endTemp, double rate, int iter, \
               int reject, char* inputfile, bool verbose );
      void run();
      void printSummary() const;
      void initTable() const;

   private:
      //constant
      int MAX_STATE_CHANGE_PER_TEMP;
      int MAX_REJECT;
      double TEMP_CHANGE_FACTOR;
      double END_TEMP;

      //variable
      State currentState;
      State bestState;
      double temp;
      double bestTemp; //temp that achieve best config
      bool verbose;

      //function
      int getCost();

      void printState(const State& state) const;
      void printStateVerbose(const State& state, const char& newStateFlag, \
                             const double& randomNum) const;
};

#endif
