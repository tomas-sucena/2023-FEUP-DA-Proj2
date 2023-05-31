#include <algorithm>

#include "DataGraph.h"

/**
 * @brief creates a new DataGraph
 * @param n number of vertices
 * 
*/
DataGraph::DataGraph(int n) : UGraph(n) {}

Path DataGraph::backtracking(){
    std::vector<Vertex *> vertices = getVertices();
    vertices.erase(vertices.begin());

    Path res;
    int dest;

    do {
        Path curr;
        int src = 1;

        for (const Vertex* v : vertices) {
            dest = v->getIndex();
            curr += getShortestPath(src, dest);

            if (curr.getWeight() >= res.getWeight()) break;
            src = dest;
        }

        if (curr.getWeight() >= res.getWeight()) continue;

        dest = 1;
        curr += getShortestPath(src, dest);

        if (curr < res) res = curr;
    } while (std::next_permutation(vertices.begin(), vertices.end()));

    return res;
}

