#ifndef __SIMULATEDANNEALING_H
#define __SIMULATEDANNEALING_H

#include <iomanip>
#include <random>
#include <algorithm>
#include <vector>

#include "DualPlanarGraphHeuristic.hpp"

template <typename T>
class SimulatedAnnealing : public DualPlanarGraphHeuristic<T>
{

private:
    double temperature;
    double alpha;
    int numIterations;
    int numDisturbance;
    int maxNumSuccess;

    DualPlanarGraph<T> disturb(DualPlanarGraph<T> &curSolution);

public:
    SimulatedAnnealing();
    ~SimulatedAnnealing();

    SimulatedAnnealing(const std::vector<std::vector<T>> &costs,
                       const std::vector<std::vector<int>> &faces,
                       double temperature,
                       double alpha,
                       int numIterations,
                       int numDisturbance,
                       int maxNumSuccess);

    virtual DualPlanarGraph<T> getBestSolution();
};

#include "../src/SimulatedAnnealing.cpp"

#endif //__SIMULATEDANNEALING_H