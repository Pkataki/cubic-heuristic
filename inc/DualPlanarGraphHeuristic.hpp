#ifndef __DUALPLANARGRAPHHEURISTIC_H
#define __DUALPLANARGRAPHHEURISTIC_H

#include "DualPlanarGraph.hpp"

template<typename T>
class DualPlanarGraphHeuristic
{
protected:
    DualPlanarGraph<T> dualPlanarGraph;


public:
    DualPlanarGraphHeuristic();
    ~DualPlanarGraphHeuristic();

    virtual DualPlanarGraph<T> getBestSolution() = 0;

}; 

#include "../src/DualPlanarGraphHeuristic.cpp"

#endif //__DUALPLANARGRAPHHEURISTIC_H