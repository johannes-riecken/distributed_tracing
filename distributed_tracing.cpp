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
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "distributed_tracing.hpp"

using namespace std;

string to_string(const optional<int> latency) {
  stringstream ss;
  if (latency) {
    ss << *latency;
  } else {
    ss << "NO SUCH TRACE";
  }
  return ss.str();
}

/* int main() { */
/*   fstream fs("graph.csv", fstream::in); */
/*   string graph_str; */
/*   fs >> graph_str; */
/*   auto g = make_unique<Graph>(Graph{graph_str}); */

/*   // exercise 1 to 5 */
/*   const array<vector<char>, 5> traces{vector<char>{'A', 'B', 'C'}, */
/*                                       {'A', 'D'}, */
/*                                       {'A', 'D', 'C'}, */
/*                                       {'A', 'E', 'B', 'C', 'D'}, */
/*                                       {'A', 'E', 'D'}}; */
/*   for (const auto &trace : traces) { */
/*     cout << to_string(g->average_latency(trace)) << endl; */
/*   } */

/*   // exercises 6 and 7 */
/*   cout << g->traces('C', 'C', 0, 3).size() << endl; */
/*   cout << g->traces('A', 'C', 4, 4).size() << endl; */

/*   // exercises 8 and 9 */
/*   const array<tuple<char, char>, 2> param_tuples{tuple<char, char>{'A', 'C'}, */
/*                                                  {'B', 'B'}}; */
/*   for (const auto &param_tuple : param_tuples) { */
/*     auto trace_candidates = g->traces(get<0>(param_tuple), get<1>(param_tuple), */
/*                                       0, Graph::NODES.size()); */
/*     auto shortest_trace_ptr = */
/*         min_element(trace_candidates.begin(), trace_candidates.end(), */
/*                     [&](vector<char> a, vector<char> b) { */
/*                       return *g->average_latency(a) < *g->average_latency(b); */
/*                     }); */
/*     cout << to_string(g->average_latency(*shortest_trace_ptr)) << endl; */
/*   } */

/*   // exercise 10 */
/*   auto short_traces = g->traces('C', 'C', 0, numeric_limits<int>().max(), 29); */
/*   cout << count_if(short_traces.begin(), short_traces.end(), [&](auto trace) { */
/*     return *g->average_latency(trace) < 30; */
/*   }) << endl; */

/*   /1* ::testing::InitGoogleTest(&argc, argv); *1/ */
/*   /1* return RUN_ALL_TESTS(); *1/ */
/* } */
