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
 * @param distance double which will be store the distance of the best path
 * @return std::list with the indices of the vertices in the order they are visited
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

/**
 * @brief computes an approximation to the TSP problem, using the triangular inequality heuristic
 * @param distance double which will be store the distance of the best path
 * @return std::list with the indices of the vertices in the order they are visited
 */
std::list<int> DataGraph::triangularInequality(double &distance) {
    std::list<int> res = {1};
    distance = 0;

    int src = 1;

    for (const Edge* e: getMST()) {
        res.push_back(e->getDest());
        distance += matrix[src][e->getDest()];

        src = e->getDest();
    }

    res.push_back(1);
    distance += matrix[src][1];

    return res;
}
