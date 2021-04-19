#ifndef __UTILS_H
#define __UTILS_H

#include <vector>
#include <set>

#include "Face.hpp"

void uniqueVector(std::vector<int> &v);

bool hasValue(std::vector<int> &v, int value);

bool hasValueSet(std::set<int> &s, int value);

int dblCmp(double a, double b);

template <typename T>
void eraseValue(std::vector<T> &v, T value)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == value)
        {
            v.erase(v.begin() + i);
            break;
        }
    }
}

template <typename T>
void merge(std::vector<T> &destine, std::vector<T> source)
{
    for (auto &element : source)
        destine.push_back(element);
}



struct PairHash
{
    template <typename T>
    size_t operator()(const std::pair<T, T> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<T>()(x.second);
    }
};

#endif // __UTILS_H