//
// Created by Tomás Sucena Lopes on 18/03/2023.
//

#include <gtest/gtest.h>
#include <string>
#include <utility>

#include "../src/implementation/DynamicPQ.hpp"

using testing::Eq;

struct Object {
    int id;
    std::string name;

    Object(int id, std::string name) : id(id), name(std::move(name)) {}

    bool operator<(const Object& o) const{
        return id < o.id;
    }
};

TEST(dynamicPQ, order_change){
    Object o1(13, "John");
    Object o2(7, "Doe");
    Object o3(41, "Eren");

    DynamicPQ<Object> pq;
    pq.push(o1);
    pq.push(o2);
    pq.push(o3);

    EXPECT_EQ("Doe", pq.peek().name);

    // modify an object
    pq.notify(o3);
    o3.id = 1;
    pq.update();

    EXPECT_EQ("Eren", pq.peek().name);
}
