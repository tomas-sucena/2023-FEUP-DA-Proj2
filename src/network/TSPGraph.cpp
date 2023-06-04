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
 * @brief calculates the great-circle distance between two vertices, using Haversine's formula
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return distance between the two vertices
 */
double TSPGraph::haversine(int src, int dest) {
    if (!isReal) return INF;

    auto &lhs = (Place &) (*this)[src];
    auto &rhs = (Place &) (*this)[dest];

    double srcLat = lhs.getLatitude();
    double destLat = rhs.getLatitude();

    double dLat = (rhs.getLatitude() - lhs.getLatitude());
    double dLong = (rhs.getLongitude() - lhs.getLongitude());

    // convert to radians
    srcLat *= M_PI / 180;
    destLat *= M_PI / 180;
    dLat *= M_PI / 180;
    dLong *= M_PI / 180;

    // apply formula
    double haver = pow(sin(dLat / 2), 2) + pow(sin(dLong / 2), 2) *
                   cos(srcLat) * cos(destLat);
    double sine = 2 * asin(sqrt(haver));

    return 6371000 * sine; // 6371000 -> Earth's radius (in meters)
}

void TSPGraph::twoOpt(std::vector<int> &indices){
    int size = (int) indices.size();
    int curr = indices.front();

    double currLength = 0;
    for (int i : indices) {
        currLength += matrix[curr][i];
        curr = i;
    }

    bool improved = true;
    while (improved){
        improved = false;

        for (int i = 1; i <= indices.size() - 2; i++) {
            for (int j = i + 1; j < indices.size() - 1; j++) {
                double newLength = - matrix[i][(i + 1) % size] - matrix[j][(j + 1) % size]
                                   + matrix[i][j] + matrix[(i + 1) % size][(j + 1) % size];

                if (newLength >= 0) continue;
                std::reverse(begin(indices) + i + 1, begin(indices) + j + 1);

                currLength += newLength;
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
 * @param src index of the source vertex
 * @return std::list representing a path, in which each entry contains the index of a vertex and the distance from the
 * previous vertex to it
 */
std::list<std::pair<int, double>> TSPGraph::nearestNeighbour(int src) {
    if (matrix.empty()) matrix = toMatrix();
    int curr = src;

    // set up the algorithm
    resetAll();

    // execute Nearest-Neighbour to obtain the initial path
    std::vector<int> indices;

    while (indices.size() < countVertices() - 1) {
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

        indices.emplace_back(nearest);
        curr = nearest;
    }

    // use 2-opt to improve the initial path
    twoOpt(indices);

    // compute the final path
    std::list<std::pair<int, double>> path;

    curr = src;
    for (int i : indices) {
        path.emplace_back(i, matrix[curr][i]);
        curr = i;
    }

    path.emplace_back(src, matrix[curr][src]);
    return path;
}
