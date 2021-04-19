template <typename T>
TabuSearch<T>::TabuSearch() {}

template <typename T>
TabuSearch<T>::~TabuSearch() {}

template <typename T>
TabuSearch<T>::TabuSearch(const std::vector<std::vector<T>> &costs,
                          const std::vector<std::vector<int>> &faces,
                          int numIterations)
{
    this->dualPlanarGraph = DualPlanarGraph(costs, faces);
    this->numIterations = numIterations;
}

// template <typename T>
// DualPlanarGraph<T> TabuSearch<T>::getBestSolution()
// {
//     auto curSolution = this->dualPlanarGraph;
//     DualPlanarGraph<T> bestSolution;
//     std::cout << curSolution.getCost() << std::endl;
//     for (int iteration = 1; iteration <= numIterations; iteration++)
//     {
//         DualPlanarGraph<T> bestNeighbor = curSolution;
//         T bestNeighborValue = -1;
//         std::vector<DualPlanarGraph<T>> neighborSolutions = curSolution.getNeighborSolutions();

//         for (const auto &neighborSolution : neighborSolutions)
//         {
//             if (tabuTable.count(neighborSolution.getEdges()))
//                 //if(tabuTable.count(neighborSolution))
//                 continue;
//             else if (neighborSolution.getCost() > bestNeighborValue)
//             {
//                 bestNeighborValue = neighborSolution.getCost();
//                 bestNeighbor = neighborSolution;
//             }
//         }
//         curSolution = bestNeighbor;
//         std::cout << iteration << " -> " << curSolution.getCost() << " -- " << bestSolution.getCost() << std::endl;

//         tabuTable[curSolution.getEdges()] = iteration;
//         //tabuTable[curSolution] = iteration;
//         if (curSolution.getCost() > bestSolution.getCost())
//             bestSolution = curSolution;
//     }
//     std::cout << bestSolution.getCost() << std::endl;
//     return bestSolution;
// }

// template <typename T>
// DualPlanarGraph<T> TabuSearch<T>::getBestSolution()
// {
//     const int maxTableSize = numIterations;
//     auto curSolution = this->dualPlanarGraph;
//     DualPlanarGraph<T> bestSolution;
//     std::cout << curSolution.getCost() << std::endl;
//     std::multiset<DualPlanarGraph<T>> topSolutions;
//     topSolutions.insert(curSolution);
//     std::map<int, std::vector<std::pair<int, int>>> revMap;
//     for (int iteration = 1; iteration <= numIterations; iteration++)
//     {
//         curSolution = *topSolutions.rbegin();
//         topSolutions.erase(prev(topSolutions.end()));

//         auto neighborSolutions = curSolution.getBestKNeighbors(tabuTable, 2);
//         for (const auto &neighborSolution : neighborSolutions)
//         {
//             if (tabuTable.count(neighborSolution.getEdges()))
//                 continue;
//             topSolutions.insert(neighborSolution);
//         }

//         while (topSolutions.size() > 5)
//             topSolutions.erase(topSolutions.begin());
//         std::cout << iteration << " -> " << curSolution.getCost() << " -- " << bestSolution.getCost() << std::endl;

//         tabuTable[curSolution.getEdges()] = iteration;
//         revMap[iteration] = curSolution.getEdges();

//         if (tabuTable.size() > maxTableSize)
//         {
//             auto removedGraph = *revMap.begin();
//             tabuTable.erase(removedGraph.second);
//             revMap.erase(revMap.begin());
//         }

//         if (curSolution.getCost() > bestSolution.getCost())
//             bestSolution = curSolution;
//     }
//     std::cout << bestSolution.getCost() << std::endl;
//     return bestSolution;
// }

double getRandom(int l, int r)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(l, r);
    return dist(mt);
}

template <typename T>
DualPlanarGraph<T> TabuSearch<T>::getBestSolution()
{
    auto curDualPlanar = this->dualPlanarGraph;
    DualPlanarGraph<T> bestSolution = curDualPlanar;
    std::cout << curDualPlanar.getCost() << std::endl;
    
    for (int iteration = 1; iteration <= numIterations; iteration++)
    {
        auto dualPlanarLocalSearch = localSearch(curDualPlanar, 200);
        std::cout << dualPlanarLocalSearch.getCost() << " -- " << curDualPlanar.getCost() << std::endl;
        if(dualPlanarLocalSearch.getCost() <= curDualPlanar.getCost())
        {
            int k = getRandom(3, 10);
            while(k--)
                curDualPlanar = curDualPlanar.getRandomNeighbor();
            iteration--;
            continue;
        }
        else 
        {
            curDualPlanar = dualPlanarLocalSearch;
        }
        
        tabuTable[curDualPlanar.getEdges()] = iteration;
        std::cout << iteration << " -> " << curDualPlanar.getCost() << " -- " << bestSolution.getCost() << std::endl;
        if (curDualPlanar.getCost() > bestSolution.getCost())
            bestSolution = curDualPlanar;
    }
    std::cout << bestSolution.getCost() << std::endl;
    return bestSolution;
}

template <typename T>
DualPlanarGraph<T> TabuSearch<T>::localSearch(DualPlanarGraph<T> curDualPlanar, const int &iterations)
{
    // DualPlanarGraph<T> bestDualPlanar = curDualPlanar;
    //std::cout << "Begin: " << bestDualPlanar.getCost() << std::endl;

    // for (int iteration = 1; iteration <= iterations; iteration++)
    // {
    //     auto bestNeighbor = curDualPlanar.getBestKNeighbors(tabuTable, 1);
    //     curDualPlanar = bestNeighbor[0];
    //     //std::cout << iteration << " -> " << curDualPlanarGraph.getCost() << " -- " << bestDualPlanarGraph.getCost() << std::endl;
    //     tabuTable[curDualPlanar.getEdges()] = iteration;
        
    //     if (curDualPlanar.getCost() > bestDualPlanar.getCost())
    //         bestDualPlanar = curDualPlanar;
    // }
    auto curSolution = curDualPlanar;
    DualPlanarGraph<T> bestDualPlanar = curDualPlanar;
    // std::cout << curSolution.getCost() << std::endl;
    std::multiset<DualPlanarGraph<T>> topSolutions;
    topSolutions.insert(curSolution);
    std::map<std::vector<std::pair<int, int>>, int> table;
    
    for (int iteration = 1; iteration <= iterations; iteration++)
    {
        curSolution = *topSolutions.rbegin();
        topSolutions.erase(prev(topSolutions.end()));

        auto neighborSolutions = curSolution.getBestKNeighbors(table, 2);
        for (const auto &neighborSolution : neighborSolutions)
        {
            if (table.count(neighborSolution.getEdges()))
                continue;
            topSolutions.insert(neighborSolution);
        }

        while (topSolutions.size() > 5)
            topSolutions.erase(topSolutions.begin());
        // std::cout << iteration << " -> " << curSolution.getCost() << " -- " << bestSolution.getCost() << std::endl;

        table[curSolution.getEdges()] = iteration;


        if (curSolution.getCost() > bestDualPlanar.getCost())
            bestDualPlanar = curSolution;
    }
    //std::cout << "End: " << bestDualPlanar.getCost() << std::endl;
    return bestDualPlanar;
}
