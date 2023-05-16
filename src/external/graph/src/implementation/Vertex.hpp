//
// Created by Tomás Sucena Lopes on 05/02/2023.
//

#ifndef GRAPH_VERTEX_HPP
#define GRAPH_VERTEX_HPP

#include <limits>
#include <list>

#include "Edge.hpp"

#define INF std::numeric_limits<double>::max()

class Vertex {
    friend class Graph;
    friend class DGraph;
    friend class UGraph;

/* ATTRIBUTES */
protected:
    int index;
    double dist;
    std::list<Edge*> out;
    std::list<Edge*> in;

public:
    bool valid;

/* CONSTRUCTOR */
protected:
    Vertex(const Vertex& v) = default;

public:
    explicit Vertex(bool valid = true)
            : valid(valid), index(0), dist(INF) {}

/* DESTRUCTOR */
public:
    virtual ~Vertex() = default;

/* METHODS */
public:
    /**
     * @brief creates a new pointer with a copy of the current object
     * @return pointer with a copy of the current object
     */
    virtual Vertex* clone() const{
        return new Vertex(*this);
    }

    /**
     * @brief returns the number of ingoing edges of the vertex (i.e. edges whose destination is the vertex)
     * @return number of ingoing edges of the vertex
     */
    int inDegree() const{
        return (int) in.size();
    }

    /**
     * @brief returns the number of outgoing edges of the vertex (i.e. edges whose source is the vertex)
     * @return number of outgoing edges of the vertex
     */
    int outDegree() const{
        return (int) out.size();
    }

    /**
     * @brief returns the ingoing edges of the vertex (i.e. edges whose destination is the vertex)
     * @return list of ingoing edges of the vertex
     */
    std::list<Edge*> inEdges() const{
        return in;
    }

    /**
     * @brief returns the outgoing edges of the vertex (i.e. edges whose source is the vertex)
     * @return list of outgoing edges of the vertex
     */
    std::list<Edge*> outEdges() const{
        return out;
    }

    bool operator<(const Vertex& rhs) const{
        if (dist == rhs.dist)
            return index < rhs.index;

        return dist < rhs.dist;
    }

    bool operator>(const Vertex& rhs) const{
        if (dist == rhs.dist)
            return index > rhs.index;

        return dist > rhs.dist;
    }
};

#endif //GRAPH_VERTEX_HPP
