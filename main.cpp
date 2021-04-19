#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>

#include "./inc/SimulatedAnnealing.hpp"
#include "./inc/CustomSearch.hpp"

#include <omp.h>

using namespace std;
using namespace boost;
using namespace std::chrono;

int mat[30][30];

typedef adjacency_list<vecS, vecS, undirectedS,
                       property<vertex_index_t, int>,
                       property<edge_index_t, int>>
    Graph;

vector<vector<int>> faces;

struct output_visitor : public planar_face_traversal_visitor
{

  vector<int> face;

  void begin_face() { face.clear(); }
  void end_face() { faces.push_back(face); }
};

struct vertex_output_visitor : public output_visitor
{
  template <typename Vertex>
  void next_vertex(Vertex v)
  {
    int u = v;
    face.push_back(u);
  }
};

vector<pair<int, int>> edgesG;

void getRandomPlanar()
{
  random_shuffle(edgesG.begin(), edgesG.end());

  Graph g;
  for(const auto &edge : edgesG)
  {
    add_edge(edge.first, edge.second, g);
    if(!boyer_myrvold_planarity_test(g))
      remove_edge(edge.first, edge.second, g);
  }

   assert(boyer_myrvold_planarity_test(g) == true);

  property_map<Graph, edge_index_t>::type e_index = get(edge_index, g);
  graph_traits<Graph>::edges_size_type edge_count = 0;
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
      put(e_index, *ei, edge_count++);

  typedef std::vector< graph_traits<Graph>::edge_descriptor > vec_t;
  std::vector<vec_t> embedding(num_vertices(g));

  if (boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,
                              boyer_myrvold_params::embedding =
                                  &embedding[0]
                              )
      )
  {}
  faces.clear();
  vertex_output_visitor v_vis;
  planar_face_traversal(g, &embedding[0], v_vis);

}

int main()
{

  int nNodes;
  cin >> nNodes;
  vector<vector<long long>> costs(nNodes + 1, vector<long long>(nNodes + 1));

  srand(time(NULL));

  for (int i = 1; i <= nNodes; i++)
  {
    for (int j = i + 1; j <= nNodes; j++)
    {
      cin >> costs[i][j];
      costs[j][i] = costs[i][j];
      edgesG.push_back({i, j});
    }
  }

  int nFaces;
  cin >> nFaces;
  cout << nFaces << " nFaces" << endl;
  for (int i = 0; i < nFaces; i++)
  {
    int a, b, c;
    cin >> a >> b >> c;
    faces.push_back({a, b, c});
    sort(faces.back().begin(), faces.back().end());
  }
  // srand(time(NULL)); 

  double temperature = 200;
  double alpha = 0.94242;
  int numIterations = 5000;
  int numDisturbance = 100;
  int maxNumSuccess = 5;
  int ans = 0;

  // int cnt = 0;
  // while(cnt < 50)
  // {
  //   //edgesG.clear();
  //   getRandomPlanar();

  //   TabuSearch<int> ch(costs, faces, numIterations);

  //   auto best = ch.getBestSolution();
  //   cnt++;
  //   ans = max(ans, best.getCost());
  //   cout << cnt << " -> " << ans << endl;
  // }

  CustomSearch<long long> ch(costs, faces, numIterations);

  auto best = ch.getBestSolution();
  
  auto edgesG = best.getEdges();
  Graph g;
  for (const auto &edge : edgesG)
  {
    //cout << edge.first << ' ' << edge.second << endl;
    ans += costs[edge.first][edge.second];
    add_edge(edge.first, edge.second, g);
  }
  cout << ans << endl;
  assert(boyer_myrvold_planarity_test(g) == true);
  property_map<Graph, edge_index_t>::type e_index = get(edge_index, g);
  graph_traits<Graph>::edges_size_type edge_count = 0;
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
      put(e_index, *ei, edge_count++);

  typedef std::vector< graph_traits<Graph>::edge_descriptor > vec_t;
  std::vector<vec_t> embedding(num_vertices(g));

  if (boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,
                              boyer_myrvold_params::embedding =
                                  &embedding[0]
                              )
      )
  {}
  faces.clear();
  vertex_output_visitor v_vis;
  planar_face_traversal(g, &embedding[0], v_vis);
  assert(faces.size() == 2*nNodes-4);
  cout << "\n\n";
  cout << nNodes << endl;
  for (int i = 1; i <= nNodes; i++)
  {
    for (int j = i + 1; j <= nNodes; j++)
    {
      cout << costs[i][j] << ' ';
    }
    cout << endl;
  }
  cout << endl;
  cout << faces.size() << endl;
  for(const auto &face: faces)
    cout << face[0] << ' ' << face[1] << ' ' << face[2] << endl;
  // for(auto edges : best.getHash())
  // {
  //   for(auto &edge : edges)
  //   {
  //     ans += costs[edge[0]][edge[1]];
  //   }
  // }

  // cout << "HERE ---> " << ans << endl;

  return 0;
}
