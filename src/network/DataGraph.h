#ifndef DA_PROJ2_DATAGRAPH
#define DA_PROJ2_DATAGRAPH

#include <list>
#include <vector>

#include "Place.hpp"
#include "UGraph.h"

using std::vector;

class DataGraph : public UGraph {
/* CONSTRUCTOR */
public:
    explicit DataGraph(int n = 0);

/* METHODS */
private:
    std::list<std::pair<int, double>> dfs(int src, vector<vector<double>> &dists);

public:
    // TSP algorithms
    std::list<std::pair<int, double>> backtracking(int src);
    std::list<std::pair<int, double>> triangularInequality(int src);
};

#endif
