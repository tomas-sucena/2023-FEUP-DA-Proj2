//
// Created by Tomás Sucena Lopes on 03-03-2023.
//

#include <gtest/gtest.h>

#include "../src/DGraph.h"
#include "../src/UGraph.h"
#include "TestGraphs.h"

using testing::Eq;

TEST(maximum_flow, edmonds_karp){
    /* DIRECTED GRAPHS */
    DGraph g1(3);
    for (int i = 1; i <= 3; ++i)
        EXPECT_EQ(0, g1.maximumFlow(i, i));

    g1.addEdge(1, 2, 3);
    g1.addEdge(2, 3, 1);

    EXPECT_EQ(1, g1.maximumFlow(1, 3));
    EXPECT_EQ(0, g1.maximumFlow(3, 1));
    EXPECT_EQ(3, g1.maximumFlow(1, 2));
    EXPECT_EQ(0, g1.maximumFlow(2, 1));
    EXPECT_EQ(1, g1.maximumFlow(2, 3));
    EXPECT_EQ(0, g1.maximumFlow(3, 2));

    // example from DA class
    DGraph g2(4);

    g2.addEdge(1, 2, 3);
    g2.addEdge(1, 3, 1);
    g2.addEdge(2, 3, 3);
    g2.addEdge(2, 4, 2);
    g2.addEdge(3, 4, 2);

    EXPECT_EQ(4, g2.maximumFlow(1, 4));
    EXPECT_EQ(0, g2.maximumFlow(4, 1));

    // Wikipedia example
    DGraph g3(7);

    g3.addEdge(1, 2, 3);
    g3.addEdge(1, 4, 3);
    g3.addEdge(2, 3, 4);
    g3.addEdge(3, 1, 3);
    g3.addEdge(3, 4, 1);
    g3.addEdge(3, 5, 2);
    g3.addEdge(4, 5, 2);
    g3.addEdge(4, 6, 6);
    g3.addEdge(5, 2, 1);
    g3.addEdge(5, 7, 1);
    g3.addEdge(6, 7, 9);

    EXPECT_EQ(5, g3.maximumFlow(1, 7));
    EXPECT_EQ(0, g3.maximumFlow(7, 1));

    // example from DA TP exercises
    DGraph g4(6);

    g4.addEdge(1, 2, 3);
    g4.addEdge(1, 3, 2);
    g4.addEdge(2, 5, 4);
    g4.addEdge(2, 4, 3);
    g4.addEdge(2, 3, 1);
    g4.addEdge(3, 5, 2);
    g4.addEdge(4, 6, 2);
    g4.addEdge(5, 6, 3);

    EXPECT_EQ(5, g4.maximumFlow(1, 6));

    std::stringstream ss;
    for (const Edge* e : g4.getEdges())
        ss << " (" << e->getSrc() << ',' << e->getDest() << ")->" << e->getFlow() << ' ';

    EXPECT_EQ(" (2,3)->0  (1,3)->2  (3,5)->2  (4,6)->2  (1,2)->3  (2,4)->2  (5,6)->3  (2,5)->1 ", ss.str());
    
    // example from Stack Overflow
    DGraph g5(8);

    g5.addEdge(1,2,1);
    g5.addEdge(2,3,1);
    g5.addEdge(3,5,1);
    g5.addEdge(1,4,1);
    g5.addEdge(4,5,1);
    g5.addEdge(5,8,1);
    g5.addEdge(4,6,1);
    g5.addEdge(6,7,1);
    g5.addEdge(7,8,1);

    EXPECT_EQ(2, g5.maximumFlow(1, 8));

    // examples from GeeksforGeeks
    DGraph g6(4);

    g6.addEdge(1,2,8);
    g6.addEdge(1,3,10);
    g6.addEdge(2,4,2);
    g6.addEdge(3,4,3);

    EXPECT_EQ(5, g6.maximumFlow(1, 4));

    DGraph g7(6);

    g7.addEdge(1, 2, 16);
    g7.addEdge(1, 3, 12);
    g7.addEdge(2,3,10);
    g7.addEdge(2, 4, 12);
    g7.addEdge(3, 2, 4);
    g7.addEdge(3, 5, 14);
    g7.addEdge(4,3,9);
    g7.addEdge(4, 6, 20);
    g7.addEdge(5, 4, 7);
    g7.addEdge(5, 6, 4);

    EXPECT_EQ(23, g7.maximumFlow(1, 6));

    /* UNDIRECTED GRAPHS */
    UGraph g8 = TestGraphs::graph8();

    EXPECT_EQ(3, g8.maximumFlow(1, 9));
    EXPECT_EQ(3, g8.maximumFlow(9, 1));
    EXPECT_EQ(9, g8.maximumFlow(1, 4));
    EXPECT_EQ(9, g8.maximumFlow(4, 1));
    EXPECT_EQ(3, g8.maximumFlow(1, 5));
    EXPECT_EQ(3, g8.maximumFlow(5, 1));
    EXPECT_EQ(43, g8.maximumFlow(8, 9));
    EXPECT_EQ(43, g8.maximumFlow(9, 8));

    UGraph g9 = TestGraphs::graph9();

    EXPECT_EQ(7, g9.maximumFlow(1, 4));
    EXPECT_EQ(7, g9.maximumFlow(4, 1));
    EXPECT_EQ(6, g9.maximumFlow(1, 2));
    EXPECT_EQ(6, g9.maximumFlow(2, 1));
    EXPECT_EQ(6, g9.maximumFlow(2, 3));
    EXPECT_EQ(6, g9.maximumFlow(3, 2));
}
