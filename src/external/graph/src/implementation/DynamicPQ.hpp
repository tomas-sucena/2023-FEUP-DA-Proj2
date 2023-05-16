//
// Created by Tomás Sucena Lopes on 18/03/2023.
//

#ifndef GRAPH_DYNAMICPQ_HPP
#define GRAPH_DYNAMICPQ_HPP

#include <set>

template <typename T>
class DynamicPQ {
    struct PointerComparator {
        bool operator()(const T* lhs, const T* rhs){
            return *lhs < *rhs;
        }
    };

/* ATTRIBUTES */
private:
    std::set<T*, PointerComparator> queue;
    bool notified;
    typename std::set<T*>::iterator it;

/* CONSTRUCTOR */
public:
    DynamicPQ() : notified(false), it(queue.begin()) {}

/* METHODS */
public:
    bool empty(){
        return queue.empty();
    }
    
    int size(){
        return (int) queue.size();
    }
    
    T& find(T& el){
        return **queue.find(&el);
    }
    
    bool push(T& el){
        return queue.insert(&el).second;
    }
    
    T& peek(){
        return **queue.begin();
    }
    
    T& pop(){
        T& head = peek();
        queue.erase(queue.begin());
        
        return head;
    }
    
    bool notify(T& el){
        it = queue.find(&el);
        notified = (it != queue.end());
        
        return notified;
    }
    
    bool update(){
        if (!notified) return false;
        
        T* el = *it;
        queue.erase(it);
        
        notified = false;
        return queue.insert(el).second;
    }
};

#endif //GRAPH_DYNAMICPQ_HPP
