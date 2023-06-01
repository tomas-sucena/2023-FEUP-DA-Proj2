#include <algorithm>
#include <stack>

#include "DataGraph.h"

/**
 * @brief creates a new DataGraph
 * @param n number of vertices
 * 
*/
DataGraph::DataGraph(int n) : UGraph(n) {}

std::list<std::pair<int, double>> DataGraph::dfs(int src, vector<vector<double>> &dists) {
    std::list<std::pair<int, double>> path;

    std::stack<int> s;
    s.push(src);

    (*this)[src].valid = false;
    int prev = src;

    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        for (const Edge *e: (*this)[curr].outEdges()) {
            int next = e->getDest();
            if (!e->valid || !(*this)[next].valid) continue;

            s.push(next);
            (*this)[next].valid = false;
        }

        if (curr == prev) continue;

        if (dists[prev][curr] < 0)
            dists[prev][curr] = getShortestPath(prev, curr).getWeight();

        path.emplace_back(curr, dists[prev][curr]);
        prev = curr;
    }

    if (dists[prev][src] < 0)
        dists[prev][src] = getShortestPath(prev, src).getWeight();

    path.emplace_back(src, dists[prev][src]);
    return path;
}

/**
 * @brief computes the solution to the TSP problem, using a brute-force backtracking algorithm
 * @complexity O(|V|! * |V|)
 * @param distance double which will be store the distance of the best path
 * @return std::list with the indices of the vertices in the order they are visited
 */
std::list<std::pair<int, double>> DataGraph::backtracking(int src){
    std::list<std::pair<int, double>> bestPath;
    double distance = INF;

    std::vector<int> indices;
    for (int i = 1; i <= countVertices(); ++i)
        if (i != src) indices.push_back(i);

    auto matrix = toMatrix();

    do {
        int prev = src;

        std::list<std::pair<int, double>> currPath;
        double currDistance = 0;

        for (int i : indices) {
            if (matrix[prev][i] < 0)
                matrix[prev][i] = getShortestPath(prev, i).getWeight();

            currPath.emplace_back(i, matrix[prev][i]);
            currDistance += matrix[prev][i];

            if (currDistance >= distance) break;
            prev = i;
        }

        if (currDistance >= distance) continue;

        if (matrix[prev][src] < 0)
            matrix[prev][src] = getShortestPath(prev, src).getWeight();

        currPath.emplace_back(src, matrix[prev][src]);
        currDistance += matrix[prev][src];

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
std::list<std::pair<int, double>> DataGraph::triangularInequality(int src) {
    std::list<Edge *> MST = getMST(src);

    // initialize the distances matrix
    vector<vector<double>> dists(countVertices() + 1);
    for (int i = 1; i <= countVertices(); ++i) {
        dists[i].resize(countVertices() + 1, -1);
        (*this)[i].valid = true;
    }

    // set up the algorithm
    for (Edge *e: edges)
        e->valid = false;

    for (Edge *e: MST) {
        e->valid = true;
        dists[e->getSrc()][e->getDest()] = e->getWeight();
    }

    // compute the path
    return dfs(src, dists);
}
