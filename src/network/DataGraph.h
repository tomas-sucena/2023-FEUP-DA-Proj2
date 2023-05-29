#ifndef DA_PROJ2_DATAGRAPH
#define DA_PROJ2_DATAGRAPH

#include "../external/graph/src/UGraph.h"
#include "Place.hpp"


class DataGraph : public UGraph{
    public: 
        explicit DataGraph(int n = 0);
        void prim();

};


#endif
