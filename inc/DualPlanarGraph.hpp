#ifndef __DUALPLANARGRAPH_H
#define __DUALPLANARGRAPH_H

#include <vector>
#include <set>
#include <map>
#include <unordered_set>

#include "Star.hpp"
#include "Face.hpp"
#include "utils.hpp"

template <typename T>
class DualPlanarGraph
{
private:
    std::vector<std::vector<T>> costs;
    std::vector<Star<T>> stars;
    std::vector<Face<T>> faces;
    std::unordered_map<Face<T>, int, FaceHash> indexStar;
    std::unordered_set<std::pair<int, int>, PairHash> edgesPrimalGraph;

    Face<T> makeFace(const std::vector<int> &face);
    std::vector<Star<T>> buildStars(const std::vector<Face<T>> &faces);
    Star<T> generateRegular5Star(const Star<T> &star);
    std::vector<Star<T>> generate5StarsByRotation(const Star<T> &star);
    std::vector<Star<T>> generateAllPossibleStarsOnRemoving(const Star<T> &star);
    std::vector<Star<T>> generate6StarsType1(const Star<T> &star);
    std::vector<Star<T>> generate6StarsType2(const Star<T> &star);
    std::vector<Star<T>> generate6StarsType3(const Star<T> &star);
    std::vector<Star<T>> generate6StarsType4(const Star<T> &star);
    DualPlanarGraph<T> buildDualPlanarSolution(const std::vector<Face<T>> &faces);
    DualPlanarGraph<T> rebuildDualPlanar(const Star<T> &starToBeRemoved,
                                         const Star<T> &starToBeInserted);

    bool checkIfCanRebuildDualPlanar(const Star<T> &starToBeRemoved,
                                     const Star<T> &starToBeInserted);

    std::vector<std::vector<int>> convertFacesToVector(const std::vector<Face<T>> &faces);

public:
    DualPlanarGraph(const std::vector<std::vector<T>> &costs,
                    const std::vector<std::vector<int>> &faces);
    DualPlanarGraph() {}
    ~DualPlanarGraph() {}

    std::vector<Star<T>> getStars() const;
    T getCost() const;
    std::vector<DualPlanarGraph<T>> getNeighborSolutions();
    DualPlanarGraph<T> getRandomNeighbor();
    std::vector<DualPlanarGraph<T>> getBestKNeighbors(int k);
    std::vector<DualPlanarGraph<T>> getBestKNeighbors(const std::map<std::vector<std::pair<int, int>>, int> &tabuTable, int k);
    std::vector<DualPlanarGraph<T>> getCustomBestKNeighbors(std::map<std::vector<std::pair<int, int>>, int> &tabuTable, int k, const int &iteration);
    std::vector<std::pair<int, int>> getEdges() const;
    int getSize() const;
};

template <typename T>
bool operator<(const DualPlanarGraph<T> &a, const DualPlanarGraph<T> &b);

template <typename T>
bool operator ==(const DualPlanarGraph<T> &a, const DualPlanarGraph<T> &b);

#include "../src/DualPlanarGraph.cpp"

#endif //__CUBICHEURISTIC_H
