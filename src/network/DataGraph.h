#ifndef DA_PROJ2_DATAGRAPH
#define DA_PROJ2_DATAGRAPH

#include "Place.hpp"
#include "UGraph.h"

class DataGraph : public UGraph{
    public: 
        explicit DataGraph(int n = 0);
        void prim();
        double backtracking();

};

#endif
