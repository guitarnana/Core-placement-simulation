#ifndef ROUTER_HPP
#define ROUTER_HPP

#define MAX_TURNS 8
#define NO_CORE -1

enum Turn {
	TOP_BOTTOM,
	BOTTOM_TOP,
	LEFT_RIGHT,
	RIGHT_LEFT,
	LEFT_BOTTOM,
	LEFT_TOP,
	RIGHT_BOTTOM,
	RIGHT_TOP
};

class Router {
   public:
      Router();
      ~Router();

      int getTurn(int t); //get turns[t]
      void changeTurn(int t, int op);
      bool isPsudonode();
      int getCoreIndex();
      void setCore(int index);

   private:
      //variable
      int turns[MAX_TURNS];
      int coreIndex;
      bool psudonode;

      //function
      void checkPsudonode();
};

inline bool Router::isPsudonode() {
   return psudonode;
}

inline int Router::getCoreIndex() {
   return coreIndex;
}

#endif
