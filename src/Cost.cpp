#include <iostream>
#include <iomanip>
#include <cmath>

#include "Cost.hpp"
#include "Utils.hpp"

using namespace std;

Cost::Cost() {}

Cost::~Cost() {}

void Cost::init(double alpha, double beta, double gamma, double theta) {
   this->alpha = alpha;
   this->beta = beta;
   this->gamma = gamma;
   this->theta = theta;

}

double Cost::getCost() {
   return cost;
}

void Cost::initCost(int** bandwidth, int** latency, vector<Core> core, const int LINK_LATENCY, Network& network) {
   compaction = compactionCost(bandwidth, core);
   dilation = dilationCost(bandwidth, latency, core, LINK_LATENCY, network);
   cost = alpha * compaction + (1-alpha) * dilation;
}

double Cost::compactionCost(int** bandwidth, vector<Core> core) {
   double sum = 0;
   for(unsigned int i = 0; i < core.size(); i++) {
      for(unsigned int j = 0; j < core.size(); j++) {
         if(bandwidth[i][j] != 0) {
            sum += bandwidth[i][j] * getHops(core[i].getPosition(),core[j].getPosition());
         }
      }
   }
   return sum;
}

double Cost::dilationCost(int** bandwidth, int** latency, vector<Core> core, const int LINK_LATENCY, Network& network) {
   slack = slackCost(latency, core, LINK_LATENCY);
   proximity = proximityCost(bandwidth, core);
   utilization = utilizationCost(bandwidth, core, network);
   return beta * slack + gamma * proximity + theta * utilization;
}

double Cost::slackCost(int** latency, vector<Core> core, const int LINK_LATENCY) {
   double sum = 0;
   int hops;
   for(unsigned int i = 0; i < core.size(); i++) {
      for(unsigned int j = 0; j < core.size(); j++) {
         if(latency[i][j] != 0) {
            hops = getHops(core[i].getPosition(),core[j].getPosition());
            sum += latency[i][j] - hops * LINK_LATENCY;
         }
      }
   }
   return sum;
}

double Cost::proximityCost(int** bandwidth, vector<Core> core) {
   double sum = 0;
   int dist;
   for(unsigned int i = 0; i < core.size(); i++) {
      for(unsigned int j = i+1; j < core.size(); j++) {
         //check that there are no connections between core i and core j
         if((bandwidth[i][j] == 0) && (bandwidth[j][i] == 0)) {
            dist = getHops(core[i].getPosition(), core[j].getPosition());
            sum -= dist;
         }
      }
   }
   return sum;
}

double Cost::utilizationCost(int** bandwidth, vector<Core> core, Network& network) {
   network.updateNetwork(bandwidth, core);
   return network.calculateUtilization();
}

void Cost::calculateCost(int** bandwidth, vector<Core> core, Network& network) {
   utilization = utilizationCost(bandwidth, core, network);
   dilation = beta * slack + gamma * proximity + theta * utilization;
   cost = alpha * compaction + (1-alpha) * dilation;
}

void Cost::updateCost(int** bandwidth, int** latency, int LINK_LATENCY, vector<Core> core, int index, int op) {
   if( op == REMOVE ) {
      compaction -= compactionCost(bandwidth, core, index);
      slack -= slackCost(latency, core, LINK_LATENCY, index);
      proximity -= proximityCost(bandwidth, core, index);
   } else {
      compaction += compactionCost(bandwidth, core, index);
      slack += slackCost(latency, core, LINK_LATENCY, index);
      proximity += proximityCost(bandwidth, core, index);
   }
}

double Cost::compactionCost(int** bandwidth, vector<Core> core, int index) {
   double change = 0;
   for(unsigned int i = 0; i < core.size(); i++) {
      //connection from index to node i
      if(bandwidth[index][i] != 0) {
         change += bandwidth[index][i] * getHops(core[index].getPosition(),core[i].getPosition());
      }
      //connection from node i to index
      if(bandwidth[i][index] != 0) {
         change += bandwidth[i][index] * getHops(core[i].getPosition(),core[index].getPosition());
      }
   }
   return change;
}

double Cost::slackCost(int** latency, vector<Core> core, const int LINK_LATENCY, int index) {
   double change = 0;
   int hops;
   for(unsigned int i = 0; i < core.size(); i++) {
      //connection from index to node i
      if(latency[index][i] != 0) {
         hops = getHops(core[index].getPosition(),core[i].getPosition());
         change += latency[index][i] - hops * LINK_LATENCY;
      }
      //connection from node i to index
      if(latency[i][index] != 0) {
         hops = getHops(core[i].getPosition(),core[index].getPosition());
         change += latency[i][index] - hops * LINK_LATENCY;
      }
   }
   return change;
}

double Cost::proximityCost(int** bandwidth, vector<Core> core, int index) {
   double change = 0;
   int dist;
   for(unsigned int i = 0; i < core.size(); i++) {
      if( i != (unsigned int)index) {
         if(bandwidth[index][i] == 0 && bandwidth[i][index] == 0) {
            dist = getHops(core[i].getPosition(), core[index].getPosition());
            change -= dist;
         }
      }
   }
   return change;
}


void Cost::printCost() {
   cout << right << setiosflags(ios::fixed) << setprecision(3)
   << setw(12) << cost
   << setw(12) << compaction
   << setw(12) << dilation
   << setw(12) << slack
   << setw(12) << proximity
   << setw(12) << utilization;
}

void Cost::printSummary() {
   cout << setiosflags(ios::fixed) << setprecision(3)<< "Cost: " << cost
        << "\tCompaction: " << compaction
        << "\tDilation: " << dilation << endl
        << "Slack: " << slack
        << "\tProximity: " << proximity
        << "\tUtilization: " << utilization << endl;
}
