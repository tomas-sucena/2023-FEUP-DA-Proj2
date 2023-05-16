//
// Created by Tomás Sucena Lopes on 05/02/2023.
//

#ifndef GRAPH_EDGE_HPP
#define GRAPH_EDGE_HPP

class Edge {
    friend class Graph;
    friend class DGraph;
    friend class UGraph;

/* ATTRIBUTES */
protected:
    int src;
    int dest;
    double weight;
    double flow;

public:
    bool valid;

/* CONSTRUCTORS */
protected:
    Edge(const Edge& e) = default;

public:
    Edge(int src, int dest, double weight, bool valid)
            : src(src), dest(dest), weight(weight), valid(valid), flow(0) {}

/* DESTRUCTOR */
public:
    virtual ~Edge() = default;

/* METHODS */
public:
    /**
     * @brief creates a new pointer with a copy of the current object
     * @return pointer with a copy of the current object
     */
    virtual Edge* clone() const{
        return new Edge(*this);
    }

    int getSrc() const{
        return src;
    }

    int getDest() const{
        return dest;
    }

    double getWeight() const{
        return weight;
    }

    double getFlow() const{
        return flow;
    }

    bool operator<(const Edge& e) const{
        if (weight != e.weight)
            return weight < e.weight;

        if (src != e.src)
            return src < e.src;

        return dest < e.dest;
    }
};

namespace std {
    template <>
    struct less<Edge*> {
        bool operator()(const Edge* lhs, const Edge* rhs) const{
            return *lhs < *rhs;
        }
    };
}

#endif //GRAPH_EDGE_HPP
