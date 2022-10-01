#include <string>
#include <vector>
#include <optional>
#include <array>
#include <map>

using namespace std;

class Graph {
public:
  static constexpr array<const char, 5> NODES{{'A', 'B', 'C', 'D', 'E'}};
  map<string, int> graph{};

  explicit Graph(string &edges_str);

  [[nodiscard]] optional<int> average_latency(const vector<char> &trace) const;

  [[nodiscard]] vector<vector<char>> traces(char start_node, char end_node, int min_hops,
                              int max_hops,
                              int max_latency = numeric_limits<int>::max()) const;
};


