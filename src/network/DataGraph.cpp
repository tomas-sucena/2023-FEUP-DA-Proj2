#include <algorithm>

#include "DataGraph.h"

/**
 * @brief creates a new DataGraph
 * @param n number of vertices
 * 
*/
DataGraph::DataGraph(int n) : UGraph(n) {}

/**
 * @brief initializes the adjacency matrix which will be used in the algorithms
 */
void DataGraph::setMatrix() {
    matrix = toMatrix(true);
}

/**
 * @brief computes a solution to the TSP problem, using a brute-force backtracking algorithm
 * @complexity O(|V|! * |V|)
 * @param distance double which will be filled with the distance of the best path
 * @return path that represents the best path for the T
 */
std::list<int> DataGraph::backtracking(double &distance){
    std::list<int> bestPath;
    distance = INF;

    std::vector<int> indices;
    for (int i = 2; i <= countVertices(); ++i)
        indices.push_back(i);

    do {
        int src = 1;

        std::list<int> currPath = {src};
        double currDistance = 0;

        bool finishedPath = true;

        for (int i : indices) {
            currPath.push_back(i);
            currDistance += matrix[src][i];

            if (currDistance >= distance){
                finishedPath = false;
                break;
            }

            src = i;
        }

        if (!finishedPath) continue;

        currPath.push_back(1);
        currDistance += matrix[src][1];

        if (currDistance >= distance) continue;

        bestPath = currPath;
        distance = currDistance;
    } while (std::next_permutation(indices.begin(), indices.end()));

    return bestPath;
}

