#ifndef DA_PROJ2_DATAGRAPH
#define DA_PROJ2_DATAGRAPH

#include <list>
#include <vector>

#include "Place.hpp"
#include "UGraph.h"

using std::vector;

class DataGraph : public UGraph {
/* ATTRIBUTES */
private:
    vector<vector<double>> matrix;

/* CONSTRUCTOR */
public:
    explicit DataGraph(int n = 0);

/* METHODS */
public:
    void setMatrix();

    // TSP algorithms
    std::list<int> backtracking(double &distance);
    std::list<int> triangularInequality(double &distance);
};

#endif
