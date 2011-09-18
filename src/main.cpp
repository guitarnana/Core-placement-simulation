#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#include "Defs.hpp"
#include "Simulator.hpp"
#include "SFMT/SFMT.h"

using namespace std;

void printUsage() {
   cout << "\nusage: ./sa [options] input_file\n\n"
         << "option lists are:\n"
         << "\t-a <value> : setting alpha value (default = 1)\n"
         << "\t-b <value> : setting beta value (default = 1)\n"
         << "\t-g <value> : setting gamma value (default = 0.2)\n"
         << "\t-d <value> : setting delta value (default = 0.04)\n"
         << "\t-s <value> : setting initial temperature (default = 1000)\n"
         << "\t-e <value> : setting final threshold temperature (default = 0.1)\n"
         << "\t-r <value> : setting temperature reduction rate (default = 0.9)\n"
         << "\t-t <value> : setting iterations per temperature (default = 400)\n"
         << "\t-c <value> : setting number of consecutive rejection per temperature (default = 200)\n"
         << "\t-p <value> : setting threshold of state accept per temperature (default = 100)\n"
         << "\t-n <value> : setting seed value for random number\n"
         << "\t-o <file>  : specify output of the simulation in an input format "
         << "that can be used as an input for next simulation\n"
         << "\t-v         : verbose printing\n"
         << "\t-q         : quiet printing\n"
         << "\t-h         : print usage\n\n";
}

int main(int argc, char* argv[]) {

   unsigned int seed = time(NULL);
   int c;
   double alpha = ALPHA;
   double beta = BETA;
   double gamma = GAMMA;
   double delta = DELTA;
   double start = S_TEMP;
   double end = E_TEMP;
   double rate = RATE;
   int iter = ITER;
   int reject = REJECT;
   int accept = ACCEPT;
   bool verbose = false;
   bool quiet = false;
   char* inputfile = NULL;
   char* outfile = NULL;
   string outstr;

   if (argc == 1) {
      printUsage();
      return 0;
   }

   while ((c = getopt(argc, argv, "a:b:g:d:s:e:r:i:c:p:n:hvqo:")) != -1) {
      switch (c) {
      case 'a':
         alpha = atof(optarg);
         break;
      case 'b':
         beta = atof(optarg);
         break;
      case 'g':
         gamma = atof(optarg);
         break;
      case 'd':
         delta = atof(optarg);
         break;
      case 's':
         start = atof(optarg);
         break;
      case 'e':
         end = atof(optarg);
         break;
      case 'r':
         rate = atof(optarg);
         break;
      case 'i':
         iter = atoi(optarg);
         break;
      case 'c':
         reject = atoi(optarg);
         break;
      case 'p':
         accept = atoi(optarg);
         break;
      case 'n':
         seed = (unsigned int) atoi(optarg);
         break;
      case 'v':
         verbose = true;
         break;
      case 'q':
         quiet = true;
         break;
      case 'h':
         printUsage();
         return 0;
      case 'o':
         outfile = optarg;
         break;
      case '?':
         cout << "Unknown arguments\n";
         printUsage();
         break;
      }
   }

   inputfile = argv[optind];

   init_gen_rand(seed);

   /*
    * Initialize simulated annealing
    */
   Simulator sa;
   int err = sa.init(alpha, beta, gamma, delta, start, end, rate, iter, reject,
         accept, inputfile, verbose, quiet);

   if (err == FILE_OPEN_ERR) {
      cout << "# File open error exit" << endl;
      return 0;
   } else if (err == ILLEGAL_STATE_ERR) {
      cout << "# Illegal initial state" << endl;
      sa.printIllegalConnection();
      return 0;
   }

   /*
    * verbose or normal output printing
    * print seed number and initial state
    */
   if (!quiet) {
      cout << "# Random number seed " << seed << endl;
      cout << "# Initial State" << endl;
      sa.printSummary();
      cout << "#" << endl;
      sa.initTable();
   }

   /*
    * start simulated annealing
    */
   sa.run();

   /*
    * verbose or normal printing
    * - print summary and diagram of final state.
    *
    * quiet printing
    * - print seed, parameters and costs
    */
   if (!quiet) {
      cout << endl;
      sa.printSummary();
      sa.printLatencyTable();
   } else {
      stringstream s;
      s << seed << " ";
      s << setw(3) << alpha << setw(5) << beta << setw(5) << gamma << setw(5) << delta << setw(7)
            << start << setw(7) << end << setw(7) << rate;
      s << sa.printFinalCost();
      cout << s.str();
   }

   /*
    * Generate output in an input format
    * so that it can be used as input for simulator
    */
   if (outfile != NULL) {
      sa.generateOutput(outfile);
   }

   return 0;
}
