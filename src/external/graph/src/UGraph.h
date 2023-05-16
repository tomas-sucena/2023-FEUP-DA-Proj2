//
// Created by Tomás Sucena Lopes on 20/02/2023.
//

#ifndef GRAPH_UGRAPH_H
#define GRAPH_UGRAPH_H

#include <list>

#include "implementation/Graph.h"

using std::list;

class UGraph : public Graph {
/* CONSTRUCTOR */
public:
    explicit UGraph(int n = 0);

/* METHODS */
public:
    bool addEdge(Edge* e) override;
    bool addEdge(int src, int dest, double weight = 1, bool valid = true) override;
    bool removeEdge(int src, int dest) override;

    bool isDirected() const override;
    UGraph getSubgraph(list<int> vertexIndices);

    list<list<int>> getConnectedComponents();
    int countConnectedComponents();

    list<int> getArticulationPoints();
    int countArticulationPoints();

    list<Edge*> getMST();
};

#endif //GRAPH_UGRAPH_H
