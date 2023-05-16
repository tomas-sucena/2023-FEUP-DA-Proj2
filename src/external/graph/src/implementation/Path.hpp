//
// Created by Tomás Sucena Lopes on 06/03/2023.
//

#ifndef GRAPH_PATH_HPP
#define GRAPH_PATH_HPP

#include <list>

#include "Edge.hpp"

class Path {
/* ATTRIBUTES */
private:
    std::list<const Edge*> edges;
    std::list<int> indices;

/* CONSTRUCTORS */
public:
    Path() = default;

    explicit Path(int src) {
        indices.push_back(src);
    }

/* METHODS */
public:
    std::list<int> getIndices() const{
        return indices;
    }

    auto begin() const {
        return edges.begin();
    }

    auto end() const{
        return edges.end();
    }

    const Edge* front() const{
        return edges.front();
    }

    const Edge* back() const{
        return edges.back();
    }

    bool push_back(const Edge* e){
        if (!indices.empty() && e->getSrc() != indices.back()) return false;
        edges.push_back(e);

        if (indices.empty()) indices.push_back(e->getSrc());
        indices.push_back(e->getDest());

        return true;
    }

    bool push_front(const Edge* e){
        if (!indices.empty() && e->getDest() != indices.front()) return false;
        edges.push_front(e);

        if (indices.empty()) indices.push_front(e->getDest());
        indices.push_front(e->getSrc());

        return true;
    }
};

#endif //GRAPH_PATH_HPP
