template <typename T>
CustomSearch<T>::CustomSearch() {}

template <typename T>
CustomSearch<T>::~CustomSearch() {}

template <typename T>
CustomSearch<T>::CustomSearch(const std::vector<std::vector<T>> &costs,
                              const std::vector<std::vector<int>> &faces,
                              int numIterations)
{
    this->dualPlanarGraph = DualPlanarGraph(costs, faces);
    this->numIterations = numIterations;
}

template <typename T>
DualPlanarGraph<T> CustomSearch<T>::getBestSolution()
{
  auto curSolution = this->dualPlanarGraph;
  DualPlanarGraph<T> bestSolution = curSolution;
  std::multiset<DualPlanarGraph<T> >bagSolutions;
  std::set<std::vector<std::pair<int,int> > >table, visited;
  for(int iteration = 1; iteration <= numIterations; iteration++)
  {
    auto neighbors = curSolution.getCustomBestKNeighbors(tabuTable, 50, iteration);
    DualPlanarGraph<T>nextSolution;
    bool hasNeighbor = false;
    for(int i = 0; i < neighbors.size(); i++)
    {
      if(!table.count(neighbors[i].getEdges()))
      {
        nextSolution = neighbors[i];
        hasNeighbor = true;
        break;
      }
    }

    for(int i = 0; i < neighbors.size(); i++)
    {
      if(table.count(neighbors[i].getEdges()))
        continue;
      table.insert(neighbors[i].getEdges());
      bagSolutions.insert(neighbors[i]);
      while(bagSolutions.size() > 50)
        bagSolutions.erase(bagSolutions.begin());
    }
    
    if(!hasNeighbor)
    {
      auto test = curSolution;
      int cnt = 0;
      int cntEqual = 0;
      do
      {
        curSolution = *(std::prev(bagSolutions.end()));
        bagSolutions.erase(std::prev(bagSolutions.end()));
        cnt++;
        cntEqual += (test == curSolution);
      }while(visited.count(curSolution.getEdges()));

    }
    else
    {
      curSolution = nextSolution; 
    }
    visited.insert(curSolution.getEdges());
    //std::cout << iteration << " -> " << curSolution.getCost() << " -- " << bestSolution.getCost() << std::endl;

    if(curSolution.getCost() > bestSolution.getCost())
    {
      bestSolution = curSolution;
    }
    //std::cout << "ending" << std::endl;
  }
  return bestSolution;
}