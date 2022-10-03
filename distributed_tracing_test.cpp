#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <optional>
#include "distributed_tracing.hpp"

using namespace std;

class GraphTest : public ::testing::Test {
protected:
  Graph<vector<pair<pair<char, char>, int>>::iterator, char> g;
  static constexpr array<const char, 5> NODES{{'A', 'B', 'C', 'D', 'E'}};

    GraphTest() :
            g{Graph<vector<pair<pair<char, char>, int>>::iterator, char>{from_edges_str("AB5,BC4,CD8,DC8,DE6,AD5,CE2,EB3,AE7"s)}}
    {
    };
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// 1. The average latency of the trace A-B-C.
TEST_F(GraphTest, ex1) {
  ASSERT_EQ(g.average_latency(vector<char>{'A', 'B', 'C'}), 9);
}

// 2. The average latency of the trace A-D.
TEST_F(GraphTest, ex2) {
  ASSERT_EQ(g.average_latency(vector<char>{'A', 'D'}), 5);
}

// 3. The average latency of the trace A-D-C.
TEST_F(GraphTest, ex3) {
  ASSERT_EQ(g.average_latency(vector<char>{'A', 'D', 'C'}), 13);
}

// 4. The average latency of the trace A-E-B-C-D.
TEST_F(GraphTest, ex4) {
  ASSERT_EQ(g.average_latency(vector<char>{'A', 'E', 'B', 'C', 'D'}), 22);
}

// 5. The average latency of the trace A-E-D.
TEST_F(GraphTest, ex5) {
  ASSERT_EQ(g.average_latency(vector<char>{'A', 'E', 'D'}), nullopt);
}

// 6. The number of traces originating in service C and ending in service C * with
// a maximum of 3 hops. In the sample data below there are two such traces:
// C-D-C (2 stops) and C-E-B-C (3 stops).
TEST_F(GraphTest, ex6) { ASSERT_EQ(g.traces('C', 'C', 0, 3).size(), 2); }

// 7. The number of traces originating in A and ending in C with exactly 4 * hops.
// In the sample data below there are three such traces: A to C (via B, C, * D); A
// to C (via D, C, D); and A to C (via D, E, B).
TEST_F(GraphTest, ex7) { ASSERT_EQ(g.traces('A', 'C', 4, 4).size(), 3); }

// 8. The length of the shortest trace (in terms of latency) between A and C.
TEST_F(GraphTest, ex8) {
  auto traces = g.traces('A', 'C', 0, NODES.size());
  auto shortest_trace_ptr = min_element(
      traces.begin(), traces.end(), [&](const vector<char>& a, const vector<char>& b) {
        return g.average_latency(a) < g.average_latency(b);
      });
  auto min_latency = g.average_latency(*shortest_trace_ptr);
  ASSERT_EQ(min_latency, 9);
}

// 9. The length of the shortest trace (in terms of latency) between B and B.
TEST_F(GraphTest, ex9) {
  auto traces = g.traces('B', 'B', 0, NODES.size());
  auto shortest_trace_ptr =
      min_element(traces.begin(), traces.end(), [&](auto a, auto b) {
        return g.average_latency(a) < g.average_latency(b);
      });
  auto min_latency = g.average_latency(*shortest_trace_ptr);
  ASSERT_EQ(min_latency, 9);
}

// 10. The number of different traces from C to C with an average latency of
// less than 30. In the same data, the traces are C-D-C, C-E-B-C, * C-E-B-C-D-C,
// C-D-C-E-B-C, C-D-E-B-C, C-E-B-C-E-B-C, C-E-B-C-E-B-C-E-B-C.
TEST_F(GraphTest, ex10) {
  auto traces = g.traces('C', 'C', 0, numeric_limits<int>::max(), 29);
  auto count = count_if(traces.begin(), traces.end(), [&](auto trace) {
    return g.average_latency(trace) < 30;
  });
  ASSERT_EQ(count, 7);
}
