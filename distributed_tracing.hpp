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

  [[nodiscard]] optional<int> average_latency(const vector<char> &trace) const {
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

  [[nodiscard]] vector<vector<char>> traces(const char start_node, const char end_node, const int min_hops,
                              const int max_hops,
                              const int max_latency = numeric_limits<int>::max()) const {
    vector<vector<char>> frontier{{start_node}};
    vector<vector<char>> ret{};
    int n_hops = 0;
    while (n_hops < max_hops) {
      // for all node lists in the frontier, add all node lists with the new
      // nodes connected by edges added if they don't have a too high average
      // latency
      // check if we should use emplace and move semantics
      vector<vector<char>> new_frontier{};
      for (const auto& nodes : frontier)
        for (const auto dest : NODES) {
          vector<char> nodes_copy{nodes};
          nodes_copy.emplace_back(dest);
          new_frontier.emplace_back(nodes_copy);
        }


      new_frontier.erase(remove_if(new_frontier.begin(), new_frontier.end(), [this, &max_latency](const auto& nodes) {
          return graph.find(""s + nodes[nodes.size() - 2] + nodes.back()) == graph.end() || *average_latency(nodes) > max_latency;
          }), new_frontier.end());
      frontier = new_frontier;
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


