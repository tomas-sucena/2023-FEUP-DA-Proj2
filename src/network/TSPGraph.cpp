#include <algorithm>
#include <math.h>
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

    return 6371 * sine; // 6371 -> Earth's radius
}

std::list<std::pair<int, double>> TSPGraph::dfs(int src, vector<vector<double>> &dists) {
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
            dists[prev][curr] = isReal ? haversine(prev, curr)
                                       : distance(prev, curr);

        path.emplace_back(curr, dists[prev][curr]);
        prev = curr;
    }

    if (dists[prev][src] < 0)
        dists[prev][src] = isReal ? haversine(prev, src)
                                  : distance(prev, src);

    path.emplace_back(src, dists[prev][src]);
    return path;
}

/**
 * @brief computes the solution to the TSP problem, using a brute-force backtracking algorithm
 * @complexity O(|V|! * |V|)
 * @param distance double which will be store the distance of the best path
 * @return std::list with the indices of the vertices in the order they are visited
 */
std::list<std::pair<int, double>> TSPGraph::backtracking(int src){
    std::list<std::pair<int, double>> bestPath;
    double minDistance = INF;

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
 * @param distance double which will be store the distance of the best path
 * @return std::list with the indices of the vertices in the order they are visited
 */
std::list<std::pair<int, double>> TSPGraph::triangularInequality(int src) {
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
