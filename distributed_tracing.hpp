#include <string>
#include <vector>
#include <optional>
#include <array>
#include <map>
#include <concepts>
#include <unordered_map>

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

template <class Iterator, regular Vertex>
class Graph {
public:

  explicit Graph<void, Vertex>(vector<pair<pair<Vertex, Vertex>, int>> &edges);

  [[nodiscard]] optional<int> average_latency(const vector<Vertex> &trace) const;

  [[nodiscard]] vector<vector<Vertex>> traces(Vertex start_node, Vertex end_node, int min_hops,
                              int max_hops,
                              int max_latency = numeric_limits<int>::max()) const;

  [[nodiscard]] vector<Vertex> vertices() const;
private:
    unordered_map<pair<Vertex, Vertex>, int, pair_hash> graph{};

};

Graph<set<pair<pair<char, char>, int>>, char> from_edges_str(string &edges_str);
