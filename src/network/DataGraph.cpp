#include "DataGraph.h"

/**
 * @brief creates a new DataGraph
 * @param n number of vertices
 * 
*/
DataGraph::DataGraph(int n) : UGraph(n) {}

double DataGraph::backtracking(){
    std::vector<Vertex *> nodes = (*this).vertices;
    nodes.erase(nodes.begin());
    double max_weight = INF;
    int dest;
    do {
        int total_dist = 0;
        int src = 1;
        for(auto it : nodes){
            dest = it->getIndex();
            total_dist += getShortestPath(src, dest).getWeight();
            if(total_dist > max_weight) break;
            src = dest;
        }
        if(total_dist < max_weight){
            dest = 1;
            total_dist += dijkstra(src, dest).getWeight();
            if(total_dist < max_weight) max_weight = total_dist;
        }
    } while (std::next_permutation(nodes.begin(), nodes.end()));

    return max_weight;
}

