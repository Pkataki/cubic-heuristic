#ifndef __CUSTOMSEARCH_H
#define __CUSTOMSEARCH_H

#include <map>
#include <random>
#include <set>

#include "DualPlanarGraphHeuristic.hpp"

template <typename T>
class CustomSearch : public DualPlanarGraphHeuristic<T>
{

private:
    int numIterations;
    std::map<std::vector<std::pair<int, int>>, int> tabuTable;

public:
    CustomSearch();
    ~CustomSearch();

    CustomSearch(const std::vector<std::vector<T>> &costs,
                 const std::vector<std::vector<int>> &faces,
                 int numIterations);

    virtual DualPlanarGraph<T> getBestSolution();
};

#include "../src/CustomSearch.cpp"

#endif //__CUSTOMSEARCH_H