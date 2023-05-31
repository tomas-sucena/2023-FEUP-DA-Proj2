#include <algorithm>

#include "DataGraph.h"

/**
 * @brief creates a new DataGraph
 * @param n number of vertices
 * 
*/
DataGraph::DataGraph(int n) : UGraph(n) {}

Path DataGraph::backtracking(){
    Path res;

    std::vector<Vertex *> vertices = getVertices();
    vertices.erase(vertices.begin());

    int dest;
    double minDistance = INF;

    do {
        Path curr;
        int src = 1;

        bool finishedPath = true;

        for (const Vertex* v : vertices) {
            dest = v->getIndex();

            curr += getShortestPath(src, dest);

            if (curr.getWeight() >= minDistance){
                finishedPath = false;
                break;
            }

            src = dest;
        }

        if (!finishedPath) continue;

        dest = 1;
        curr += getShortestPath(src, dest);

        if (curr.getWeight() >= minDistance) continue;

        res = curr;
        minDistance = res.getWeight();
    } while (std::next_permutation(vertices.begin(), vertices.end()));

    return res;
}

