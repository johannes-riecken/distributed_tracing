class Graph
  NODES = ('A' .. 'E')
  def initialize(edges)
    @graph = Hash[edges.split(',').map { |x| [x[0..1], x[2].to_i] }]
  end

  def average_latency(trace)
    edges = trace.each_cons(2).map(&:join)
    edges.map { |x| @graph[x] }.reduce { |acc, x| acc.nil? ? nil : x.nil? ? nil : acc + x } || 'NO SUCH TRACE'
  end

  def traces(start_node, end_node, min_hops, max_hops, max_latency = Float::INFINITY)
    frontier = [[start_node]]
    ret = []
    n_hops = 0
    while n_hops < max_hops
      frontier = frontier.flat_map do |nodes|
        NODES.filter { |x| @graph[nodes[-1] + x] }.map { |x| [*nodes, x] }.filter { |x| average_latency(x) <= max_latency }
      end
      break if frontier.empty?
      n_hops += 1
      if n_hops >= min_hops
        ret.append(*frontier.filter { |nodes| nodes[-1] == end_node })
      end
    end
    return ret
  end
end

require 'test/unit'
class TestGraph < Test::Unit::TestCase
  def setup
    input = 'AB5,BC4,CD8,DC8,DE6,AD5,CE2,EB3,AE7'
    @g = Graph.new(input)
  end

  # 1. The average latency of the trace A-B-C.
  def test_ex1
    assert_equal(9, @g.average_latency(['A', 'B', 'C']))
  end

  # 2. The average latency of the trace A-D.
  def test_ex2
    assert_equal(5, @g.average_latency(['A', 'D']))
  end

  # 3. The average latency of the trace A-D-C.
  def test_ex3
    assert_equal(13, @g.average_latency(['A', 'D', 'C']))
  end

  # 4. The average latency of the trace A-E-B-C-D.
  def test_ex4
    assert_equal(22, @g.average_latency(['A', 'E', 'B', 'C', 'D']))
  end

  # 5. The average latency of the trace A-E-D.
  def test_ex5
    assert_equal(@g.average_latency(['A', 'E', 'D']), 'NO SUCH TRACE')
  end

  # 6. The number of traces originating in service C and ending in service C with a maximum of
  # 3 hops. In the sample data below there are two such traces: C-D-C (2 stops) and
  # C-E-B-C (3 stops).
  def test_ex6
    assert_equal(2, @g.traces('C', 'C', 0, 3).count)
  end

  # 7. The number of traces originating in A and ending in C with exactly 4 hops. In the sample
  # data below there are three such traces: A to C (via B, C, D); A to C (via D, C, D); and A
  # to C (via D, E, B).
  def test_ex7
    p @g.traces('A', 'C', 4, 4)
    assert_equal(3, @g.traces('A', 'C', 4, 4).count)
  end
  # 8. The length of the shortest trace (in terms of latency) between A and C.
  def test_ex8
    assert_equal(9, @g.traces('A', 'C', 0, Graph::NODES.count).map { |x| @g.average_latency(x) }.min)
  end
  # 9. The length of the shortest trace (in terms of latency) between B and B.
  def test_ex9
    assert_equal(9, @g.traces('B', 'B', 0, Graph::NODES.count).map { |x| @g.average_latency(x) }.min)
  end
  # 
  # 10. The number of different traces from C to C with an average latency of less than 30. In
  # the same data, the traces are C-D-C, C-E-B-C, C-E-B-C-D-C, C-D-C-E-B-C, C-D-E-B-C,
  # C-E-B-C-E-B-C, C-E-B-C-E-B-C-E-B-C.
  def test_ex10
    assert_equal(7, @g.traces('C', 'C', 0, Float::INFINITY, 29).map { |x| @g.average_latency(x) }.filter { |x| x < 30 }.count)
  end
end
# 1. 9
# 2. 5
# 3. 13
# 4. 22
# 5. NO SUCH TRACE
# 6. 2
# 7. 3
# 8. 9
# 9. 9
# 10. 7
