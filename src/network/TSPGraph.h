#ifndef DA_PROJ2_TSPGRAPH
#define DA_PROJ2_TSPGRAPH

#include <list>
#include <vector>

#include "Place.hpp"
#include "UGraph.h"

using std::vector;

class TSPGraph : public UGraph {
/* ATTRIBUTES */
private:
    vector<vector<double>> matrix;
    bool isReal;

/* CONSTRUCTOR */
public:
    explicit TSPGraph(bool isReal = false);

/* METHODS */
private:
    double haversine(int src, int dest);
    void twoOpt(std::vector<int> &indices);

public:
    // TSP algorithms
    std::list<std::pair<int, double>> backtracking(int src);
    std::list<std::pair<int, double>> triangularInequality(int src);
    std::list<std::pair<int, double>> nearestNeighbour(int src);
};

#endif
