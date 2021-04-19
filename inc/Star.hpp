#ifndef __STAR_H
#define __STAR_H

#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "Face.hpp"
#include "utils.hpp"

template <typename T>
class Star
{
private:
    Face<T> i, j, k, l;
    std::vector<int> vertices;
    std::map<int, std::vector<int>> adj;

    void buildCycle();

public:
    Star() {}
    ~Star() {}
    Star(Face<T> i, Face<T> j, Face<T> k, Face<T> l);
    Star(const std::vector<Face<T>> &faces);

    T getCost() const;

    std::vector<Face<T>> getFaces() const;

    std::vector<Face<T>> getFacesWithoutCenter() const;

    std::vector<int> getVertices() const;

    // return the cycle formed in the star
    // if is a 5-cycle the center is in the last position
    std::vector<int> getCycle() const;

    Face<T> getCenter() const;

    std::vector<std::pair<int,int> > getEdges() const;
};

template <typename T>
bool operator!=(const Star<T> starX, const Star<T> starY);

template <typename T>
bool operator==(const Star<T> &starX, const Star<T> &starY);

template <typename T>
bool operator<(const Star<T> starX, const Star<T> starY);

#include "../src/Star.cpp"

#endif // __STAR_H
