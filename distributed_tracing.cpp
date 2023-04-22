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
#include <iterator>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_set>

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

pair<vector<pair<pair<char, char>, int>>::const_iterator,
        vector<pair<pair<char, char>, int>>::const_iterator> parse_edges_str(string_view edges_str, vector<pair<pair<char, char>, int>>& edges_parsed) {
    vector<string> edges{};
    string token;
    string edges_str_new;
    ranges::copy(edges_str, back_inserter(edges_str_new));
    ranges::replace(edges_str_new, ',', ' ');
    istringstream ss{edges_str_new};
    istream_iterator<string> it{ss};
    copy(it, istream_iterator<string>{}, back_inserter(edges));
    ranges::transform(edges, back_inserter(edges_parsed), [](const string &e) {
        char a = e[0];
        char b = e[1];
        return make_pair<pair<char, char>, int>(pair<char, char>(a, b), stoi(e.substr(2)));
    });
    return {edges_parsed.cbegin(), edges_parsed.cend()};
}

template<input_iterator EdgeIterator, input_iterator VertexIterator, regular Vertex>
Graph<EdgeIterator, VertexIterator, Vertex>::Graph(const EdgeIterator& ei_begin, const EdgeIterator& ei_end) : graph{ei_begin, ei_end}
{
}

Graph<vector<pair<pair<char, char>, int>>::const_iterator, vector<char>::const_iterator, char> from_edges_str(const string &edges_str) {
    vector<pair<pair<char, char>, int>> edges_parsed{};
    auto [ei_begin, ei_end] = parse_edges_str(edges_str, edges_parsed);
    return Graph<vector<pair<pair<char, char>, int>>::const_iterator, vector<char>::const_iterator, char>{ei_begin, ei_end};
}

template <input_iterator EdgeIterator, input_iterator VertexIterator, regular Vertex>
optional<int> Graph<EdgeIterator, VertexIterator, Vertex>::average_latency(VertexIterator& trace_begin, VertexIterator& trace_end) const {
    int latency = 0;
    vector<pair<Vertex, Vertex>> edges{};
    transform(trace_begin, trace_end - 1, trace_begin + 1,
              back_inserter(edges), [](Vertex a, Vertex b) { return pair<Vertex, Vertex>(a, b); });
    auto f = ranges::find_if(edges, [&](const auto& edge){
        if (!graph.contains(edge)) {
            return true;
        }
        latency += (*graph.find(edge)).second;
        return false;
    });
    return f == edges.cend() ? optional<int>(latency) : nullopt;
}

template <input_iterator EdgeIterator, input_iterator VertexIterator, regular Vertex>
optional<int> Graph<EdgeIterator, VertexIterator, Vertex>::average_latency(const VertexIterator& trace_begin, const VertexIterator& trace_end) const {
    int latency = 0;
    vector<pair<Vertex, Vertex>> edges{};
    transform(trace_begin, trace_end - 1, trace_begin + 1,
              back_inserter(edges), [](Vertex a, Vertex b) { return pair<Vertex, Vertex>(a, b); });
    auto f = ranges::find_if(edges, [&](const auto& edge){
        if (!graph.contains(edge)) {
            return true;
        }
        latency += (*graph.find(edge)).second;
        return false;
    });
    return f == edges.cend() ? optional<int>(latency) : nullopt;
}

template <input_iterator EdgeIterator, input_iterator VertexIterator, regular Vertex>
vector<vector<Vertex>> Graph<EdgeIterator, VertexIterator, Vertex>::traces(const Vertex start_node, const Vertex end_node, const int min_hops, const int max_hops,
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


        erase_if(new_frontier, [this, &max_latency](auto& nodes) {
            auto nodes_begin = cbegin(nodes);
            auto nodes_end = cend(nodes);
            return !graph.contains(pair(nodes[nodes.size() - 2], nodes.back())) || *average_latency(nodes_begin, nodes_end) > max_latency;
        });
        frontier = new_frontier;
        if (new_frontier.empty()) {
            break;
        }

        n_hops += 1;
        if (n_hops >= min_hops) {
            vector<vector<Vertex>> filtered_frontier{};
            ranges::copy_if(frontier,
                    back_inserter(filtered_frontier),
                    [&](const auto &nodes) { return (nodes.back() == end_node); });
            ret.insert(ret.cend(), filtered_frontier.cbegin(),
                       filtered_frontier.cend());
        }
    }
    return ret;
}

template <input_iterator EdgeIterator, input_iterator VertexIterator, regular Vertex>
vector<Vertex> Graph<EdgeIterator, VertexIterator, Vertex>::vertices() const {
    unordered_set<Vertex> m{};
    ranges::transform(graph, inserter(m, begin(m)), [](const auto &p) { return p.first.first; });
    ranges::transform(graph, inserter(m, begin(m)), [](const auto &p) { return p.first.second; });
    vector<Vertex> ret{};
    ranges::copy(m, back_inserter(ret));
    return ret;
}

class A
{
    struct Rslt {
        Rslt(Rslt&&) = delete;
        void operator&() const = delete;
        friend void operator,(Rslt, Rslt) = delete;
    };

    __attribute__((noreturn)) static Rslt make() { throw 0; }

public:
    void operator&() const = delete;
    friend void operator,(A const&, A const&) = delete;
    // implement equality operators
    friend bool operator==(A const&, A const&) = default;
};

// std::hash implementation for A
namespace std {
    template <>
    struct hash<A> {
        size_t operator()(A const&) const { return 0; }
    };
}

template class Graph<vector<pair<pair<char, char>, int>>::const_iterator, vector<char>::const_iterator, char>;
