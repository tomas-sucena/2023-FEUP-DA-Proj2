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
    double minDistance = INF;

    std::vector<int> indices;
    for (int i = 1; i <= countVertices(); ++i)
        indices.push_back(i);

    // create the memoization table
    Path paths[countVertices() + 1][countVertices() + 1];

    do {
        int src = indices.front();
        if (src > 1) break;

        Path curr;
        bool finishedPath = true;

        for (int i : indices) {
            if (paths[src][i].empty())
                paths[src][i] = getShortestPath(src, i);

            curr += paths[src][i];
            if (curr.getWeight() >= minDistance){
                finishedPath = false;
                break;
            }

            src = i;
        }

        if (!finishedPath) continue;

        if (paths[src][1].empty())
            paths[src][1] = getShortestPath(src, 1);

        curr += paths[src][1];
        if (curr.getWeight() >= minDistance) continue;

        res = curr;
        minDistance = res.getWeight();
    } while (std::next_permutation(indices.begin(), indices.end()));

    return res;
}

