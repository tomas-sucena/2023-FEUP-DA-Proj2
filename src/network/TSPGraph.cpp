#include <algorithm>
#include <cmath>
#include <stack>

#include "TSPGraph.h"

/**
 * @brief creates a new TSPGraph
 * @param isReal indicates if the graph represents real world locations
*/
TSPGraph::TSPGraph(bool isReal) : UGraph(0), isReal(isReal) {}

/**
 * @brief calculates the great-circle distance between two vertices, using the Haversine formula
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return distance between the two vertices (in meters)
 */
double TSPGraph::haversine(int src, int dest) {
    if (!isReal) return INF;

    auto &lhs = (Place &) (*this)[src];
    auto &rhs = (Place &) (*this)[dest];

    double srcLat = lhs.getLatitude();
    double destLat = rhs.getLatitude();

    double dLat = rhs.getLatitude() - lhs.getLatitude();
    double dLong = rhs.getLongitude() - lhs.getLongitude();

    // convert to radians
    srcLat *= M_PI / 180;
    destLat *= M_PI / 180;
    dLat *= M_PI / 180;
    dLong *= M_PI / 180;

    // apply formula
    double haver = pow(sin(dLat / 2), 2) + pow(sin(dLong / 2), 2) *
                   cos(srcLat) * cos(destLat);
    double sine = 2 * atan2(sqrt(haver), sqrt(1 - haver));

    return 6371000 * sine; // 6371000 -> Earth's radius (in meters)
}

/**
 * @brief computes an approximate solution to the TSP, using an implementation of the Nearest-Neighbours algorithm
 * @complexity O(|V| + |E|)
 * @param src index of the source vertex
 * @param distance double where the distance of the computed path will be stored
 * @return std::vector containing the indices of the vertices in the order they were visited
 */
std::vector<int> TSPGraph::nearestNeighbours(int src, double &distance) {
    std::vector<int> path;

    int curr = src;
    distance = 0;

    while (path.size() < countVertices() - 1) {
        int nearest;
        double minDistance = INF;

        (*this)[curr].valid = false;

        for (const Edge *e: (*this)[curr].outEdges()) {
            int next = e->getDest();

            if (!e->valid || !(*this)[next].valid || e->getWeight() >= minDistance)
                continue;

            minDistance = e->getWeight();
            nearest = next;
        }

        path.emplace_back(nearest);

        curr = nearest;
        distance += minDistance;
    }

    return path;
}

/**
 * @brief optimizes a path, using an implementation of the 2-optimization algorithm
 * @complexity O(|V|^2)
 * @param path std::vector containing the path (represented by the indices of the vertices) to be optimized
 * @param distance double where the distance of the initial path is stored and where the distance of the optimized
 * path will be stored
 */
void TSPGraph::twoOpt(std::vector<int> &path, double &distance){
    int size = (int) path.size();

    bool improved = true;
    while (improved){
        improved = false;

        for (int i = 0; i < size - 1; ++i) {
            for (int j = i + 1; j < size; ++j) {
                int a = path[i];
                int b = path[(i + 1) % size];
                int c = path[j];
                int d = path[(j + 1) % size];

                // calculate the current distance
                double currDistance = matrix[a][b] + matrix[c][d];

                // calculate the new distance
                double newDistance = matrix[a][c] + matrix[b][d];

                // check if the new distance is an optimization
                if (newDistance >= currDistance) continue;
                std::reverse(path.begin() + i + 1, path.begin() + j + 1);

                distance += newDistance - currDistance;
                improved = true;
            }
        }
    }
}

/**
 * @brief computes the solution to the TSP problem, using a brute-force backtracking algorithm
 * @complexity O(|V|! * |V|)
 * @param distance double which will be store the distance of the best path
 * @return std::list representing a path, in which each entry contains the index of a vertex and the distance from the
 * previous vertex to it
 */
std::list<std::pair<int, double>> TSPGraph::backtracking(int src){
    std::list<std::pair<int, double>> bestPath;
    double minDistance = INF;

    std::vector<int> indices;
    for (int i = 1; i <= countVertices(); ++i)
        if (i != src) indices.push_back(i);

    if (matrix.empty()) matrix = toMatrix();

    do {
        int prev = src;

        std::list<std::pair<int, double>> currPath;
        double currDistance = 0;

        for (int i : indices) {
            if (matrix[prev][i] < 0)
                matrix[prev][i] = distance(prev, i);

            currPath.emplace_back(i, matrix[prev][i]);
            currDistance += matrix[prev][i];

            if (currDistance >= minDistance) break;
            prev = i;
        }

        if (currDistance >= minDistance) continue;

        if (matrix[prev][src] < 0)
            matrix[prev][src] = distance(prev, src);

        currPath.emplace_back(src, matrix[prev][src]);
        currDistance += matrix[prev][src];

        if (currDistance >= minDistance) continue;

        bestPath = currPath;
        minDistance = currDistance;
    } while (std::next_permutation(indices.begin(), indices.end()));

    return bestPath;
}

/**
 * @brief computes an approximation to the TSP problem, using the triangular inequality heuristic
 * @complexity O(|V + E| * log|V|)
 * @param distance double which will be store the distance of the best path
 * @return std::list representing a path, in which each entry contains the index of a vertex and the distance from the
 * previous vertex to it
 */
std::list<std::pair<int, double>> TSPGraph::triangularInequality(int src) {
    std::list<Edge *> MST = getMST(src);
    if (matrix.empty()) matrix = toMatrix();

    // set up the algorithm
    for (Edge *e: edges)
        e->valid = false;

    for (Edge *e: MST) {
        e->valid = true;
        (*this)[e->getSrc()].valid = true;
    }

    // compute the path using DFS
    std::list<std::pair<int, double>> path;
    double (TSPGraph::*dist)(int, int) = isReal ? &TSPGraph::haversine : &TSPGraph::distance;

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

        if (matrix[prev][curr] < 0)
            matrix[prev][curr] = (this->*dist)(prev, curr);

        path.emplace_back(curr, matrix[prev][curr]);
        prev = curr;
    }

    if (matrix[prev][src] < 0)
        matrix[prev][src] = (this->*dist)(prev, src);

    path.emplace_back(src, matrix[prev][src]);
    return path;
}

/**
 * @brief implementation of the Nearest-Neighbour algorithm, which yields an approximation for the TSP
 * @complexity O(|V|^2)
 * @param src index of the source vertex
 * @return std::list representing a path, in which each entry contains the index of a vertex and the distance from the
 * previous vertex to it
 */
std::list<std::pair<int, double>> TSPGraph::other(int src) {
    if (matrix.empty()) matrix = toMatrix();

    // set up the algorithm
    resetAll();

    // execute Nearest-Neighbours to obtain the initial path
    double distance;
    std::vector<int> initialPath = nearestNeighbours(src, distance);

    // use 2-opt to optimize the path
    twoOpt(initialPath, distance);

    // compute the final path
    std::list<std::pair<int, double>> path;

    int curr = src;
    for (int i : initialPath) {
        path.emplace_back(i, matrix[curr][i]);
        curr = i;
    }

    path.emplace_back(src, matrix[curr][src]);
    return path;
}
