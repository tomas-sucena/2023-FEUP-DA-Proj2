//
// Created by Tomás Sucena Lopes on 05/02/2023.
//

#include "Graph.h"

#include <algorithm>
#include <queue>
#include <stdexcept>
#include <unordered_set>

#include "DynamicPQ.hpp"

#define uSet std::unordered_set

/**
 * @brief implementation of the Breadth-First Search algorithm, which traverses the graph
 * @complexity O(|V| + |E|)
 * @param src index of the vertex where the algorithm will start
 * @return list containing the indices of all the visited vertices
 */
std::list<int> Graph::bfs(int src){
    std::list<int> visitedVertices;

    (*this)[src].valid = false;
    (*this)[src].dist = 0;

    std::queue<int> q;
    q.push(src);

    while (!q.empty()){
        int curr = q.front();
        visitedVertices.push_back(curr);

        for (const Edge* e : (*this)[curr].out){
            int next = e->dest;

            (*this)[next].dist = std::min((*this)[curr].dist + e->weight, (*this)[next].dist);

            if (!(*this)[next].valid || !e->valid) continue;
            (*this)[next].valid = false;

            q.push(next);
        }

        q.pop();
    }

    return visitedVertices;
}

/**
 * @brief implementation of the Breadth-First Search algorithm, which finds ALL the shortest paths between two vertices in a Graph
 * @complexity O(|V| + |E|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return list containing all the shortest paths that unite the two vertices
 */
std::list<Path> Graph::bfs(int src, int dest){
    std::list<Path> allPaths = {Path(src)};

    (*this)[src].valid = false;
    (*this)[src].dist = 0;

    if (src == dest) return allPaths; // special case

    std::queue<int> q;
    q.push(src);

    while (!q.empty()){
        int curr = q.front();
        if (curr == dest) break; // destination reached

        for (const Edge* e : (*this)[curr].out){
            int next = e->dest;
            Path path = allPaths.front();

            (*this)[next].dist = std::min((*this)[curr].dist + e->weight, (*this)[next].dist);

            path.push_back(e);
            allPaths.push_back(path);

            if (!(*this)[next].valid || !e->valid) continue;
            (*this)[next].valid = false;

            q.push(next);
        }

        q.pop();
        allPaths.pop_front();
    }

    // eliminate the paths that don't end in the destination
    for (auto it = allPaths.begin(); it != allPaths.end();){
        if (it->back()->dest == dest){
            ++it;
            continue;
        }

        it = allPaths.erase(it);
    }

    return allPaths;
}

/**
 * @brief implementation of the Dijkstra algorithm, which finds the shortest path between two vertices in a Graph
 * @complexity O(|E| * log|V|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return shortest path that unites the two vertices
 */
Path Graph::dijkstra(int src, int dest){
    (*this)[src].valid = false;
    (*this)[src].dist = 0;

    if (src == dest) return Path(src); // special case

    DynamicPQ<Vertex> pq;
    pq.push((*this)[src]);

    std::vector<const Edge*> prev(vertices.size() + 1, nullptr);

    while (!pq.empty()){
        int curr = pq.pop().index;

        if (curr == dest) break;

        for (const Edge* e : (*this)[curr].out){
            int next = e->dest;

            if ((*this)[curr].dist + e->weight < (*this)[next].dist){
                // notify the PQ that we will alter an element
                pq.notify((*this)[next]);

                (*this)[next].dist = (*this)[curr].dist + e->weight;
                prev[next] = e;

                // update the PQ
                pq.update();
            }

            if (!(*this)[next].valid || !e->valid) continue;
            (*this)[next].valid = false;

            pq.push((*this)[next]);
        }
    }

    // reconstruct the shortest path
    Path path;

    for (const Edge* e = prev[dest]; e != nullptr; e = prev[e->src])
        path.push_front(e);

    return path;
}

/**
 * @brief implementation of the Edmonds-Karp algorithm, which computes the maximum flow between two vertices
 * @complexity O(|V| * |E|^2)
 * @param src index of the source vertex
 * @param sink index of the sink vertex
 * @param augPaths list where the augmenting paths will be inserted
 * @return maximum flow
 */
double Graph::edmondsKarp(int src, int sink, std::list<Path>* augPaths){
    double flow = 0;

    while (true){
        std::vector<Edge*> prev(vertices.size() + 1, nullptr);

        // BFS
        std::queue<int> q;
        q.push(src);

        while (!q.empty()){
            int curr = q.front();
            q.pop();

            if (curr == sink) break;

            // edges
            for (Edge* e : (*this)[curr].out){
                int next = e->dest;
                if (!(*this)[next].valid || !e->valid) continue;

                if (prev[next] != nullptr || next == src || e->flow >= e->weight)
                    continue;

                prev[next] = e;
                q.push(next);
            }

            // reverse edges
            for (Edge* e : (*this)[curr].in){
                int next = e->src;
                if (!(*this)[next].valid || !e->valid) continue;

                if (prev[next] != nullptr || next == src || e->flow <= 0)
                    continue;

                prev[next] = e;
                q.push(next);
            }
        }

        if (prev[sink] == nullptr) break;

        // path augmentation
        double bottleneck = INF;

        int last = sink;
        for (Edge* e = prev[last]; e != nullptr; e = prev[last]){
            if (e->dest == last){
                bottleneck = std::min(bottleneck, e->weight - e->flow);
                last = e->src;

                continue;
            }

            bottleneck = std::min(bottleneck, e->flow);
            last = e->dest;
        }

        last = sink;
        Path p;

        for (Edge* e = prev[last]; e != nullptr; e = prev[last]){
            p.push_front(e);

            if (e->dest == last){
                e->flow += bottleneck; last = e->src;
                continue;
            }

            e->flow -= bottleneck; last = e->dest;
        }

        flow += bottleneck;
        if (augPaths != nullptr) augPaths->push_back(p);
    }

    return flow;
}

/* CONSTRUCTOR */
/**
 * @brief creates a new Graph
 * @param n number of vertices that the Graph will be initialized with
 */
Graph::Graph(int n) : weighted(false), autoReset(true) {
    reserve(n);
}

/* METHODS */
/**
 * @brief adds extra empty vertices to the Graph
 * @param n number of empty vertices that will be added to the Graph
 * @return 'true' if the resize occurs, 'false' otherwise
 */
bool Graph::reserve(int n){
    if (n <= 0)
        return false;

    for (int i = 0; i < n; ++i){
        vertices.push_back(new Vertex());
        (*this)[countVertices()].index = countVertices();
    }

    return true;
}

/**
 * @brief adds a vertex to the Graph
 * @param v pointer to the vertex that will be added
 */
void Graph::addVertex(Vertex *v){
    if (v == nullptr) v = new Vertex();

    v->index = (int) vertices.size() + 1;
    vertices.push_back(v);
}

/**
 * @brief removes a vertex from the Graph
 * @complexity O(|V| * |E|)
 * @param index index of the vertex that will be removed
 * @return number of edges that were removed (those whose destination was the deleted vertex)
 */
int Graph::removeVertex(int index){
    if (index <= 0 || index > (int) vertices.size()) return -1;

    uSet<int> affectedVertices;

    // update the edges
    int deletedEdges = 0;

    for (auto it = edges.begin(); it != edges.end();){
        bool srcRemoved = ((*it)->src == index);
        bool destRemoved = ((*it)->dest == index);

        if (destRemoved)
            affectedVertices.insert((*it)->src);

        if ((*it)->dest > index)
            (*it)->dest--;

        if ((*it)->src > index)
            (*it)->src--;

        if (!srcRemoved && !destRemoved){
            ++it;
            continue;
        }

        (*it)->dest = -1;
        it = edges.erase(it);
        ++deletedEdges;
    }

    // update the vertices
    for (int i = 1; i <= vertices.size(); ++i){
        if (i > index)
            (*this)[i].index = i - 1;

        if (affectedVertices.find(i) == affectedVertices.end()) continue;

        Vertex& v = (*this)[i];
        for (auto it = v.out.begin(); it != v.out.end();){
            if ((*it)->dest > 0){
                ++it;
                continue;
            }

            delete (*it);
            it = v.out.erase(it);
        }

        for (auto it = v.in.begin(); it != v.in.end();){
            if ((*it)->dest > 0){
                ++it;
                continue;
            }

            delete (*it);
            it = v.in.erase(it);
        }
    }

    // remove the vertex
    vertices.erase(vertices.begin() + index - 1);

    return deletedEdges;
}

/**
 * @brief adds an edge to the Graph, that is, a connection between two vertices
 * @complexity O(log|E|)
 * @param e edge to be added
 * @return 'true' if the insertion occurs, 'false' otherwise
 */
bool Graph::addEdge(Edge* e){
    if (e == nullptr) return false;
    
    if (!validIndex(e->src) || !validIndex(e->dest)){
        delete e;
        return false;
    }

    if (!edges.insert(e).second) return false;

    (*this)[e->src].out.push_back(e);
    (*this)[e->dest].in.push_back(e);

    weighted += (e->weight != 1);

    return true;
}

/**
 * @brief adds an edge to the Graph, that is, a connection between two vertices
 * @complexity O(log|E|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @param weight cost of the connection
 * @param valid bool that indicates if the edge should be considered in Graph traversals
 * @return 'true' if the insertion occurs, 'false' otherwise
 */
bool Graph::addEdge(int src, int dest, double weight, bool valid){
    return addEdge(new Edge(src, dest, weight, valid));
}

/**
 * @brief removes an edge from the Graph, that is, eliminates the connection between two vertices
 * @complexity O(|E|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return 'true' if the removal occurs, 'false' otherwise
 */
bool Graph::removeEdge(int src, int dest){
    if (!areConnected(src, dest))
        return false;

    for (auto it = edges.begin(); it != edges.end();){
        if ((*it)->src != src || (*it)->dest != dest){
            ++it;
            continue;
        }

        weighted -= ((*it)->weight != 1);
        it = edges.erase(it);
    }

    // remove the edge from the outgoing edges list of the source vertex
    for (auto it = (*this)[src].out.begin(); it != (*this)[src].out.end();){
        if ((*it)->src != src || (*it)->dest != dest){
            ++it;
            continue;
        }

        it = (*this)[src].out.erase(it);
    }

    // remove the edge from the ingoing edges list of the destination vertex
    for (auto it = (*this)[dest].in.begin(); it != (*this)[dest].in.end();){
        if ((*it)->src != src || (*it)->dest != dest){
            ++it;
            continue;
        }

        it = (*this)[dest].in.erase(it);
    }

    return true;
}

/**
 * @brief indicates if the Graph is weighted
 * @return 'true' if the Graph is weighted, 'false' otherwise
 */
bool Graph::isWeighted() const{
    return (bool) weighted;
}

/**
 * @brief returns the number of vertices that the Graph currently has
 * @return number of vertices of the Graph
 */
int Graph::countVertices() const{
    return (int) vertices.size();
}

/**
 * @brief returns the number of edges that the Graph currently has
 * @return number of edges of the Graph
 */
int Graph::countEdges() const{
    return (int) edges.size();
}

/**
 * @brief returns the vector which stores the vertices of the Graph
 * @return std::vector that stores the vertices of the Graph
 */
std::vector<Vertex*> Graph::getVertices() const{
    return vertices;
}

/**
 * @brief returns the set which stores the edges of the Graph
 * @return std::set that stores the edges of the Graph
 */
std::set<Edge*> Graph::getEdges() const{
    return edges;
}

/**
 * @brief accesses a vertex of the Graph and allows modifications to be made to it
 * @param index index of the vertex
 * @return reference of the vertex
 */
Vertex& Graph::operator[](int index){
    if (!validIndex(index))
        throw std::invalid_argument("Invalid index!");

    return *vertices[index - 1];
}

/**
 * @brief verifies if there exists an edge that connects two vertices
 * @complexity O(|E|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return 'true' if the vertices are connected, 'false' otherwise
 */
bool Graph::areConnected(int src, int dest) const{
    if (!validIndex(src) || !validIndex(dest))
        return false;

    for (const Edge* e : vertices[src - 1]->out){
        if (e->dest != dest) continue;

        return true;
    }

    return false;
}

/**
 * computes the maximum distance between a vertex and all others
 * @complexity O(|V| + |E|)
 * @param index index of the vertex
 * @param farthest list where the index of the farthest vertices will be inserted
 * @return maximum distance between the vertex and all others
 */
double Graph::eccentricity(int index, std::list<int>* farthest){
    if (autoReset) resetAll();

    double ecc = 0;
    std::list<int> farthestVertices;

    for (int i : bfs(index)){
        if (ecc > (*this)[i].dist) continue;

        if (ecc < (*this)[i].dist){
            ecc = (*this)[i].dist;
            farthestVertices.clear();
        }

        farthestVertices.push_back(i);
    }

    if (farthest != nullptr) *farthest = farthestVertices;

    return ecc;
}

/**
 * computes the maximum distance between any two vertices in the Graph
 * @complexity O(|V| * (|V| + |E|))
 * @param farthest list where the indices of the farthest vertex pairs will be inserted
 * @return pair containing the diameter and the two vertices that define it
 */
double Graph::diameter(std::list<std::pair<int, int>>* farthest){
    double diameter = 0;
    std::list<std::pair<int, int>> farthestPairs;

    for (int i = 1; i <= (int) vertices.size(); ++i){
        std::list<int> farthestVertices;
        double ecc = eccentricity(i, &farthestVertices);

        if (diameter > ecc) continue;

        if (diameter < ecc){
            diameter = ecc;
            farthestPairs.clear();
        }

        for (int j : farthestVertices)
            farthestPairs.emplace_back(i, j);
    }

    if (farthest != nullptr) *farthest = farthestPairs;

    return diameter;
}

/**
 * @brief validates all the vertices
 * @complexity O(|V|)
 */
void Graph::resetVertices(){
    for (Vertex* v : vertices){
        v->valid |= autoResetSettings.vertexValid;
        if (autoResetSettings.vertexDist) v->dist = INF;
    }
}

/**
 * @brief validates all the edges
 * @complexity O(|E|)
 */
void Graph::resetEdges(){
    for (Edge* e : edges){
        e->valid |= autoResetSettings.edgeValid;
        if (autoResetSettings.edgeFlow) e->flow = 0;
    }
}

/**
 * @brief validates all the vertices and edges
 * @complexity O(|V| + |E|)
 */
void Graph::resetAll(){
    resetVertices();
    resetEdges();
}

/**
 * checks if an index is valid, i.e. it is within the number of vertices
 * @param index index that will be validated
 * @return 'true' if the index is valid, 'false' otherwise
 */
bool Graph::validIndex(int index) const{
    return (index > 0 && index <= (int) vertices.size());
}

/**
 * @brief calculates the minimum distance between two vertices
 * @complexity O(|E| * log|V|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return minimum distance between the source and the destination if they are connected, -1 otherwise
 */
double Graph::distance(int src, int dest){
    if (autoReset) resetAll();
    dijkstra(src, dest);

    double res = (*this)[dest].dist;
    res = (res == INF) ? -1 : res;

    return res;
}

/**
 * @brief calculates (one of) the shortest path between two vertices
 * @complexity O(|E| * log|V|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return list containing the indices of the vertices that form the path
 */
Path Graph::getShortestPath(int src, int dest){
    if (autoReset) resetAll();
    return dijkstra(src, dest);
}

/**
 * @brief calculates ALL the shortest paths between two vertices
 * @complexity O(|V| + |E|)
 * @param src index of the source vertex
 * @param dest index of the destination vertex
 * @return list containing the shortest paths (each path is represented by the indices of the vertices that form it)
 */
std::list<Path> Graph::getShortestPaths(int src, int dest){
    if (autoReset) resetAll();
    return bfs(src, dest);
}

/**
 * @brief calculates which vertices can be reached in a given distance from a source vertex, using an implementation of the
 * Breadth-First Search algorithm
 * @complexity O(|V| + |E|)
 * @param src index of the source vertex
 * @param dist distance
 * @param weighted bool that specifies if the given distance is referring to the weight of the edges ('true') or the
 * total number of edges ('false')
 * @return list containing the indices of the reachable vertices
 */
std::list<int> Graph::getReachable(int src, double dist, bool weighted){
    if (autoReset) resetAll();
    std::list<int> reachableVertices;

    (*this)[src].valid = false;
    (*this)[src].dist = 0;

    std::queue<std::pair<int, double>> q;
    q.push({src, dist});

    while (!q.empty()){
        int currIndex = q.front().first;
        double currDistance = q.front().second;
        q.pop();

        if (currDistance < 0) continue;
        reachableVertices.push_back(currIndex);

        for (const Edge* e : (*this)[currIndex].out){
            int nextIndex = e->dest;
            double nextDistance = currDistance - (weighted ? e->weight : 1);

            (*this)[nextIndex].dist = std::min((*this)[currIndex].dist + e->weight, (*this)[nextIndex].dist);

            if (!e->valid || !(*this)[nextIndex].valid) continue;
            (*this)[nextIndex].valid = false;

            q.push({nextIndex, nextDistance});
        }
    }

    return reachableVertices;
}

/**
 * @brief computes the maximum flow between two vertices using an implementation of the Edmonds-Karp algorithm
 * @complexity O(|V| * |E|^2)
 * @param src index of the source vertex
 * @param sink index of the sink vertex
 * @param augPaths list where the augmenting paths will be inserted
 * @return maximum flow
 */
double Graph::maximumFlow(int src, int sink, std::list<Path>* augPaths){
    if (!validIndex(src) || !validIndex(sink))
        return -1;

    if (autoReset) resetAll();
    return edmondsKarp(src, sink, augPaths);
}
