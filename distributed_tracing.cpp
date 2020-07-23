// Running the program: Create file graph.csv with one comma-separated line
// specifying a graph (without spaces). Compile this file with `g++ -std=c++17
// distributed_tracing.cpp` (I would also gladly provide instructions for
// compilation using Visual Studio). After compilation the program can be run by
// running the generated ./a.out file. The unit tests are commented out to
// simplify compilation, which would otherwise require the gtest library and the
// addition of `-lgtest` to the end of the g++ command-line parameters. If I had
// more time, I would add:
// - Error handling, especially for non-existing file and argument validation
// - Documentation comments
// - Improve the complexity of some algorithms (I'm aware of Dijkstra's
// algorithm)
// - Minor amendments like giving more variables `const` and `auto` qualifiers
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

/* #include "gtest/gtest.h" */

using namespace std;

class Graph {
public:
  static constexpr array<const char, 5> NODES{{'A', 'B', 'C', 'D', 'E'}};
  map<string, int> graph{};

  explicit Graph(string &edges_str) {
    vector<string> edges{};
    string token;
    replace(edges_str.begin(), edges_str.end(), ',', ' ');
    istringstream ss{edges_str};
    istream_iterator<string> it{ss};
    copy(it, istream_iterator<string>{}, back_inserter(edges));
    transform(edges.begin(), edges.end(), inserter(graph, graph.begin()), [](const auto &e) {
      return make_pair<string, int>(e.substr(0, 2), stoi(e.substr(2)));
    });
  }

  optional<int> average_latency(const vector<char> &trace) const {
    int latency = 0;
    vector<string> edges{};
    transform(trace.begin(), trace.end() - 1, trace.begin() + 1,
              back_inserter(edges), [](char a, char b) { return ""s + a + b; });
    auto f = find_if(edges.begin(), edges.end(), [&](const auto& edge){if (graph.find(edge) == graph.end()) {
            return true;
            }
            latency += (*graph.find(edge)).second;
            return false;
            });
    return f == edges.end() ? optional<int>(latency) : nullopt;
  }

  vector<vector<char>> traces(const char start_node, const char end_node, const int min_hops,
                              const int max_hops,
                              const int max_latency = numeric_limits<int>().max()) const {
    vector<vector<char>> frontier{{start_node}};
    vector<vector<char>> ret{};
    int n_hops = 0;
    while (n_hops < max_hops) {
      // for all node lists in the frontier, add all node lists with the new
      // nodes connected by edges added if they don't have a too high average
      // latency
      // check if we should use emplace and move semantics
      vector<vector<char>> new_frontier{frontier};
      for (auto& nodes : new_frontier)
        for (const auto& dest : NODES)
          nodes.push_back(dest);

      remove_if(new_frontier.begin(), new_frontier.end(), [this, &max_latency](const auto& nodes) {
          return graph.find(""s + nodes[nodes.size() - 2] + nodes.back()) == graph.end() || *average_latency(nodes) > max_latency;
          });
      copy(new_frontier.begin(), new_frontier.end(), back_inserter(frontier));
      if (new_frontier.empty()) {
        break;
      }

      n_hops += 1;
      if (n_hops >= min_hops) {
        vector<vector<char>> filtered_frontier{};
        copy_if(frontier.begin(), frontier.end(),
                back_inserter(filtered_frontier),
                [&](const auto &nodes) { return (nodes.back() == end_node); });
        ret.insert(ret.end(), filtered_frontier.begin(),
                   filtered_frontier.end());
      }
    }
    return ret;
  }
};

string to_string(const optional<int> latency) {
  stringstream ss;
  if (latency) {
    ss << *latency;
  } else {
    ss << "NO SUCH TRACE";
  }
  return ss.str();
}

int main() {
  fstream fs("graph.csv", fstream::in);
  string graph_str;
  fs >> graph_str;
  auto g = make_unique<Graph>(Graph{graph_str});

  // exercise 1 to 5
  const array<vector<char>, 5> traces{vector<char>{'A', 'B', 'C'},
                                      {'A', 'D'},
                                      {'A', 'D', 'C'},
                                      {'A', 'E', 'B', 'C', 'D'},
                                      {'A', 'E', 'D'}};
  for (const auto &trace : traces) {
    cout << to_string(g->average_latency(trace)) << endl;
  }

  // exercises 6 and 7
  cout << g->traces('C', 'C', 0, 3).size() << endl;
  cout << g->traces('A', 'C', 4, 4).size() << endl;

  // exercises 8 and 9
  const array<tuple<char, char>, 2> param_tuples{tuple<char, char>{'A', 'C'},
                                                 {'B', 'B'}};
  for (const auto &param_tuple : param_tuples) {
    auto trace_candidates = g->traces(get<0>(param_tuple), get<1>(param_tuple),
                                      0, Graph::NODES.size());
    auto shortest_trace_ptr =
        min_element(trace_candidates.begin(), trace_candidates.end(),
                    [&](vector<char> a, vector<char> b) {
                      return *g->average_latency(a) < *g->average_latency(b);
                    });
    cout << to_string(g->average_latency(*shortest_trace_ptr)) << endl;
  }

  // exercise 10
  auto short_traces = g->traces('C', 'C', 0, numeric_limits<int>().max(), 29);
  cout << count_if(short_traces.begin(), short_traces.end(), [&](auto trace) {
    return *g->average_latency(trace) < 30;
  }) << endl;

  /* ::testing::InitGoogleTest(&argc, argv); */
  /* return RUN_ALL_TESTS(); */
}

/* class GraphTest : public ::testing::Test { */
/* protected: */
/*   unique_ptr<Graph> g; */
/*   GraphTest() {} */
/*   void SetUp() override { */
/*     g = make_unique<Graph>(Graph{"AB5,BC4,CD8,DC8,DE6,AD5,CE2,EB3,AE7"}); */
/*   } */
/* }; */

/* // 1. The average latency of the trace A-B-C. */
/* TEST_F(GraphTest, ex1) { */
/*   ASSERT_EQ(*g->average_latency(vector<char>{'A', 'B', 'C'}), 9); */
/* } */

/* // 2. The average latency of the trace A-D. */
/* TEST_F(GraphTest, ex2) { */
/*   ASSERT_EQ(*g->average_latency(vector<char>{'A', 'D'}), 5); */
/* } */

/* // 3. The average latency of the trace A-D-C. */
/* TEST_F(GraphTest, ex3) { */
/*   ASSERT_EQ(*g->average_latency(vector<char>{'A', 'D', 'C'}), 13); */
/* } */

/* // 4. The average latency of the trace A-E-B-C-D. */
/* TEST_F(GraphTest, ex4) { */
/*   ASSERT_EQ(*g->average_latency(vector<char>{'A', 'E', 'B', 'C', 'D'}), 22);
 */
/* } */

/* // 5. The average latency of the trace A-E-D. */
/* TEST_F(GraphTest, ex5) { */
/*   ASSERT_EQ(g->average_latency(vector<char>{'A', 'E', 'D'}), nullopt); */
/* } */
/* // 6. The number of traces originating in service C and ending in service C
 * with */
/* // a maximum of 3 hops. In the sample data below there are two such traces:
 */
/* // C-D-C (2 stops) and C-E-B-C (3 stops). */
/* TEST_F(GraphTest, ex6) { ASSERT_EQ(g->traces('C', 'C', 0, 3).size(), 2); } */

/* // 7. The number of traces originating in A and ending in C with exactly 4
 * hops. */
/* // In the sample data below there are three such traces: A to C (via B, C,
 * D); A */
/* // to C (via D, C, D); and A to C (via D, E, B). */
/* TEST_F(GraphTest, ex7) { ASSERT_EQ(g->traces('A', 'C', 4, 4).size(), 3); } */
/* // 8. The length of the shortest trace (in terms of latency) between A and C.
 */
/* TEST_F(GraphTest, ex8) { */
/*   auto traces = g->traces('A', 'C', 0, Graph::NODES.size()); */
/*   auto shortest_trace_ptr = min_element( */
/*       traces.begin(), traces.end(), [&](vector<char> a, vector<char> b) { */
/*         return *g->average_latency(a) < *g->average_latency(b); */
/*       }); */
/*   auto min_latency = *g->average_latency(*shortest_trace_ptr); */
/*   ASSERT_EQ(min_latency, 9); */
/* } */
/* // 9. The length of the shortest trace (in terms of latency) between B and B.
 */
/* TEST_F(GraphTest, ex9) { */
/*   auto traces = g->traces('B', 'B', 0, Graph::NODES.size()); */
/*   auto shortest_trace_ptr = */
/*       min_element(traces.begin(), traces.end(), [&](auto a, auto b) { */
/*         return *g->average_latency(a) < *g->average_latency(b); */
/*       }); */
/*   auto min_latency = *g->average_latency(*shortest_trace_ptr); */
/*   ASSERT_EQ(min_latency, 9); */
/* } */
/* // */
/* // 10. The number of different traces from C to C with an average latency of
 */
/* // less than 30. In the same data, the traces are C-D-C, C-E-B-C,
 * C-E-B-C-D-C, */
/* // C-D-C-E-B-C, C-D-E-B-C, C-E-B-C-E-B-C, C-E-B-C-E-B-C-E-B-C. */
/* TEST_F(GraphTest, ex10) { */
/*   auto traces = g->traces('C', 'C', 0, numeric_limits<int>().max(), 29); */
/*   auto count = count_if(traces.begin(), traces.end(), [&](auto trace) { */
/*     return *g->average_latency(trace) < 30; */
/*   }); */
/*   ASSERT_EQ(count, 7); */
/* } */
/* /1* } *1/ */
