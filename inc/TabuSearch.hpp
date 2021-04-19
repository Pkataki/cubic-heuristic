#ifndef __TABUSEARCH_H
#define __TABUSEARCH_H

#include <map>
#include <random>
#include <set>

#include "DualPlanarGraphHeuristic.hpp"

template <typename T>
class TabuSearch : public DualPlanarGraphHeuristic<T>
{

private:
    int numIterations;
    std::map<std::vector<std::pair<int, int>>, int> tabuTable;
    DualPlanarGraph<T> localSearch(DualPlanarGraph<T> curDualPlanarGraph, const int &iterations);


public:
    TabuSearch();
    ~TabuSearch();

    TabuSearch(const std::vector<std::vector<T>> &costs,
               const std::vector<std::vector<int>> &faces,
               int numIterations);

    virtual DualPlanarGraph<T> getBestSolution();
};

#include "../src/TabuSearch.cpp"

#endif //__TABUSEARCH_H