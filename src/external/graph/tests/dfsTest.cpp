//
// Created by Tomás Sucena Lopes on 12/02/2023.
//

#include <gtest/gtest.h>
#include <set>

#include "../src/DGraph.h"
#include "../src/UGraph.h"
#include "TestGraphs.h"

using testing::Eq;

TEST(dfs, isDAG){
    // DAGs
    DGraph g4 = TestGraphs::graph4();
    DGraph g6 = TestGraphs::graph6();
    DGraph g10 = TestGraphs::graph10();

    EXPECT_TRUE(g4.isDAG());
    EXPECT_TRUE(g6.isDAG());
    EXPECT_TRUE(g10.isDAG());

    // directed graphs with cycles
    DGraph g5 = TestGraphs::graph5();
    EXPECT_FALSE(g5.isDAG());

    g5.removeEdge(1, 2);
    EXPECT_TRUE(g5.isDAG());

    DGraph g12 = TestGraphs::graph12();
    EXPECT_FALSE(g12.isDAG());
}