#include <iostream>

#include "SimulatedAnnealing.hpp"

using namespace std;

int main(int argc, char* argv[]) {

   if (argc != 2) {
      cout << "usage ./program <input file>" << endl;
      return 0;
   }

   SimulatedAnnealing sa;

   int err = sa.init(argv[1]);
   if(err) {
      cout << "Error Exit" << endl;
      return 0;
   }

   sa.run();

   sa.printResult();

   return 0;
}
