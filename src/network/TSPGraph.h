#ifndef DA_PROJ2_DATAGRAPH
#define DA_PROJ2_DATAGRAPH

#include <list>
#include <vector>

#include "Place.hpp"
#include "UGraph.h"

using std::vector;

class TSPGraph : public UGraph {
/* ATTRIBUTES */
private:
    bool isReal;

/* CONSTRUCTOR */
public:
    explicit TSPGraph(bool isReal = false);

/* METHODS */
private:
    double haversine(int src, int dest);

public:
    // TSP algorithms
    std::list<std::pair<int, double>> backtracking(int src);
    std::list<std::pair<int, double>> triangularInequality(int src);
    std::list<std::pair<int, double>> nearestNeighbour(int src);
    void twoopt(std::vector<Edge*> path);
};

#endif
