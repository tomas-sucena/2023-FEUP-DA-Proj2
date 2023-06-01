#ifndef DA_PROJ2_DATAGRAPH
#define DA_PROJ2_DATAGRAPH

#include <list>

#include "Place.hpp"
#include "UGraph.h"

class DataGraph : public UGraph {
/* ATTRIBUTES */
private:
    double **matrix;

/* CONSTRUCTOR */
public:
    explicit DataGraph(int n = 0);

/* DESTRUCTOR */
public:
    ~DataGraph();

/* METHODS */
public:
    std::list<int> backtracking(double &distance);
};

#endif
