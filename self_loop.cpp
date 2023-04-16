//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee,
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp>

#include <iostream>
#include <vector>

using namespace boost;

void foo() {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    for (auto i : v) {
        std::cout << i << std::endl;
    }
    auto x = std::back_inserter(v);
    *x = 4;
    x++;
}

template <typename VertexDescriptor, typename VertexNameMap>
void print_vertex_name(VertexDescriptor v, VertexNameMap name_map) {
    std::cout << get(name_map, v); }

template <typename Graph, typename VertexNameMap>
void print_vertex_names(const Graph& g, VertexNameMap name_map) {
    std::cout << "vertices(g) = { ";
    typedef typename graph_traits<Graph>::vertex_iterator iter_t;
    for (std::pair<iter_t, iter_t> p = vertices(g); p.first != p.second; ++p.first) {
        print_vertex_name(*p.first, name_map); std::cout << ' '; }
    std::cout << "}" << std::endl; }

// record_visitors
// ===============

class record_visitors : public default_bfs_visitor {
public:
    // m_discovered is the iterator for recording the visited nodes
    std::back_insert_iterator<std::vector<int> > m_discovered;

    // m_fin
    on_tree_edge m_fin;

    // event_filter
    typedef on_tree_edge event_filter;

    // call operator
    template <typename Edge, typename Graph>
    void operator()(Edge e, Graph& g) {
         // record the visited node
        *m_discovered++ = e.m_source;
    }

//    template <typename Vertex, typename Graph>
//    void discover_vertex(Vertex u, Graph& g) {
//        *m_discovered++ = u;
//    }
    // constructor
    record_visitors(std::back_insert_iterator<std::vector<int> > &discovered, on_tree_edge fin)
        : m_discovered(discovered), m_fin(fin) {}
    // discover_vertex
};

template <typename Graph>
void bfs_to_vector(const Graph& g, std::vector<int>& v) {
    std::back_insert_iterator<std::vector<int> > discovered(v);
    breadth_first_search(g, 0, visitor(make_bfs_visitor(record_visitors(discovered, on_tree_edge()))));
}

extern "C" {
void LLVMFuzzerTestOneInput(const uint8_t *data, size_t size, uint8_t *out, size_t out_size) {
    using namespace boost;
    // initialize graph g
    typedef adjacency_list<vecS, vecS, undirectedS> graph_t;
    graph_t g;
    if (size < 2) {
        return;
    }
    for (int i = 0; i < size - 1; i++) {
        add_edge(data[i], data[i + 1], g);
    }
    int i = 0;
    for (auto it = vertices(g); it.first != it.second; ++it.first) {
        out[i++] = *it.first;
    }
}
}

int main() {
    uint8_t arr[] = {0,1,6,3,4};
    // assign length of arr to len
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);
    int max_idx = *std::max_element(arr, arr + arr_len);
    uint8_t out[max_idx + 1];
    LLVMFuzzerTestOneInput(reinterpret_cast<const uint8_t *>(arr), arr_len, reinterpret_cast<uint8_t *>(out), max_idx);
    for (int i = 0; i < max_idx + 1; i++) {
        std::cout << (int)out[i] << std::endl;
    }
//    std::vector<int> v;
//    print_vertex_names(g, get(vertex_index, g));
//    bfs_to_vector(g, v);
//    for (auto i : v) {
//        std::cout << i << std::endl;
//    }
//    uint8_t a[] = {0,1};
//    std::cout << LLVMFuzzerTestOneInput(a, 2);
}
