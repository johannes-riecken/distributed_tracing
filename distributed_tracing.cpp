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
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <tuple>
#include <unordered_set>
#include <set>
#include <iterator>

#include "distributed_tracing.hpp"

using namespace std;

[[maybe_unused]] string to_string(const optional<int> latency) {
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
pair<vector<pair<pair<char, char>, int>>::iterator,
        vector<pair<pair<char, char>, int>>::iterator> parse_edges_str(const string &edges_str) {
    vector<string> edges{};
    string token;
    string edges_str_new;
    copy(edges_str.begin(), edges_str.end(), back_inserter(edges_str_new));
    replace(edges_str_new.begin(), edges_str_new.end(), ',', ' ');
    istringstream ss{edges_str_new};
    istream_iterator<string> it{ss};
    copy(it, istream_iterator<string>{}, back_inserter(edges));
    vector<pair<pair<char, char>, int>> edges_parsed{};
    transform(edges.begin(), edges.end(), back_inserter(edges_parsed), [](const string &e) {
        char a = e[0];
        char b = e[1];
        return make_pair<pair<char, char>, int>(pair<char, char>(a, b), stoi(e.substr(2)));
    });
    return {edges_parsed.begin(), edges_parsed.end()};
}

template<input_iterator Iterator, regular Vertex>
Graph<Iterator, Vertex>::Graph(Iterator ei_begin, Iterator ei_end) : graph{ei_begin, ei_end}
{
}

Graph<vector<pair<pair<char, char>, int>>::iterator, char> from_edges_str(const string &edges_str) {
    auto [ei_begin, ei_end] = parse_edges_str(edges_str);
    return Graph<vector<pair<pair<char, char>, int>>::iterator, char>{ei_begin, ei_end};
}

template <input_iterator Iterator, regular Vertex>
optional<int> Graph<Iterator, Vertex>::average_latency(const vector<Vertex> &trace) const {
    int latency = 0;
    vector<pair<Vertex, Vertex>> edges{};
    transform(trace.begin(), trace.end() - 1, trace.begin() + 1,
              back_inserter(edges), [](Vertex a, Vertex b) { return pair<Vertex, Vertex>(a, b); });
    auto f = find_if(edges.begin(), edges.end(), [&](const auto& edge){
        if (graph.find(edge) == graph.end()) {
            return true;
        }
            latency += (*graph.find(edge)).second;
            return false;
        });
    return f == edges.end() ? optional<int>(latency) : nullopt;
}

template <input_iterator Iterator, regular Vertex>
vector<vector<Vertex>> Graph<Iterator, Vertex>::traces(const Vertex start_node, const Vertex end_node, const int min_hops, const int max_hops,
                                   const int max_latency) const {
    vector<vector<Vertex>> frontier{{start_node}};
    vector<vector<Vertex>> ret{};
    int n_hops = 0;
    while (n_hops < max_hops) {
        // for all node lists in the frontier, add all node lists with the new
        // nodes connected by edges added if they don't have a too high average
        // latency
        // check if we should use emplace and move semantics
        vector<vector<Vertex>> new_frontier{};
        for (const auto& nodes : frontier)
            for (const auto dest : vertices()) {
                vector<Vertex> nodes_copy{nodes};
                nodes_copy.emplace_back(dest);
                new_frontier.emplace_back(nodes_copy);
            }


        new_frontier.erase(remove_if(new_frontier.begin(), new_frontier.end(), [this, &max_latency](const auto& nodes) {
            return graph.find(pair(nodes[nodes.size() - 2], nodes.back())) == graph.end() || *average_latency(nodes) > max_latency;
        }), new_frontier.end());
        frontier = new_frontier;
        if (new_frontier.empty()) {
            break;
        }

        n_hops += 1;
        if (n_hops >= min_hops) {
            vector<vector<Vertex>> filtered_frontier{};
            copy_if(frontier.begin(), frontier.end(),
                    back_inserter(filtered_frontier),
                    [&](const auto &nodes) { return (nodes.back() == end_node); });
            ret.insert(ret.end(), filtered_frontier.begin(),
                       filtered_frontier.end());
        }
    }
    return ret;
}

template <input_iterator Iterator, regular Vertex>
vector<Vertex> Graph<Iterator, Vertex>::vertices() const {
    unordered_set<Vertex> m{};
    transform(graph.begin(), graph.end(), inserter(m, begin(m)), [](const auto &p) { return p.first.first; });
    transform(graph.begin(), graph.end(), inserter(m, begin(m)), [](const auto &p) { return p.first.second; });
    vector<Vertex> ret{};
    copy(m.begin(), m.end(), back_inserter(ret));
    return ret;
}

class A
{
    struct Rslt {
        /* operator A() { return {}; } */
        Rslt(Rslt&&) = delete;
        void operator&() const = delete;
        friend void operator,(Rslt, Rslt) = delete;
    };

    static Rslt make() { throw 0; }

public:
//    A() = delete;
//    A(A&&) = delete;
//    A(A const&) = delete;
//    A& operator=(A &&) = delete;
//    A& operator=(A const&) = delete;
    /* ~A() = delete; */
    /* A() = default; */
    /* A(A&&) = default; */
    /* A(A const&) = default; */
    /* A& operator=(A &&) = default; */
    /* A& operator=(A const&) = default; */
    /* ~A() = default; */

    void operator&() const = delete;
    friend void operator,(A const&, A const&) = delete;
    // implement equality operators
    friend bool operator==(A const&, A const&) = default;

//    A& operator+=(A const&) { return *this; }
//    friend Rslt operator+(A, A) { return make(); }
//
//    friend auto operator<=>(A const&, A const&) = default;
};

// std::hash implementation for A
namespace std {
    template <>
    struct hash<A> {
        size_t operator()(A const&) const { return 0; }
    };
}

template class Graph<set<pair<pair<char, char>, int>>::iterator, char>;
template class Graph<vector<pair<pair<char, char>, int>>::iterator, char>;
template class Graph<set<pair<pair<A, A>, int>>::iterator, A>;