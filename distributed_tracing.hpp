#include <string>
#include <vector>
#include <optional>
#include <array>
#include <map>
#include <concepts>
#include <unordered_map>
#include <limits>
#include <compare>

using namespace std;

// from boost (functional/hash):
// see http://www.boost.org/doc/libs/1_35_0/doc/html/hash/combine.html template
template <class T> inline void hash_combine(size_t &seed, T const &v) {
    seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const {
        size_t seed = 0;
        hash_combine(seed, p.first);
        hash_combine(seed, p.second);
        return seed;
    }
};

template <input_iterator EdgeIterator, input_iterator VertexIterator, regular Vertex>
class Graph {
public:

  explicit Graph<EdgeIterator, VertexIterator, Vertex>(const EdgeIterator& ei_begin, const EdgeIterator& ei_end);

    [[nodiscard]] optional<int> average_latency(VertexIterator& trace_begin, VertexIterator& trace_end) const;
    [[nodiscard]] optional<int> average_latency(const VertexIterator& trace_begin, const VertexIterator& trace_end) const;

  [[nodiscard]] vector<vector<Vertex>> traces(Vertex start_node, Vertex end_node, int min_hops,
                              int max_hops,
                              int max_latency = numeric_limits<int>::max()) const;

  [[nodiscard]] vector<Vertex> vertices() const;
private:
    unordered_map<pair<Vertex, Vertex>, int, pair_hash> graph{};

};

Graph<vector<pair<pair<char, char>, int>>::const_iterator, vector<char>::const_iterator, char> from_edges_str(const string &edges_str);
