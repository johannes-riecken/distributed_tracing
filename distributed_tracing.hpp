#include <string>
#include <vector>
#include <optional>
#include <array>
#include <map>

using namespace std;

template <class Vertex>
class Graph {
public:

  explicit Graph<Vertex>(string &edges_str);

  [[nodiscard]] optional<int> average_latency(const vector<char> &trace) const;

  [[nodiscard]] vector<vector<char>> traces(char start_node, char end_node, int min_hops,
                              int max_hops,
                              int max_latency = numeric_limits<int>::max()) const;

  [[nodiscard]] vector<Vertex> vertices() const;
private:
    map<string, int> graph{};
};
