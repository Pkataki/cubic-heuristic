#ifndef __FACE_H
#define __FACE_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "utils.hpp"

template <typename T>
class Face
{

private:
    T cost;
    int a, b, c;

public:
    Face(int a, int b, int c, T cost);
    Face(std::vector<int> vertices, T cost);

    Face();
    ~Face() {}

    // methods
    T getCost() const;

    void setCost(T cost);

    bool isAdjacent(Face<T> otherFace) const;

    // return ordered vertices of a face
    std::vector<int> getVertices() const;

    bool hasEdge(int u, int v) const;

    std::vector<std::pair<int,int> > getEdges() const;
};

template <typename T>
bool operator==(const Face<T> &faceX, const Face<T> &faceY);

template <typename T>
bool operator!=(const Face<T> &faceX, const Face<T> &faceY);

template <typename T>
bool operator<(const Face<T> &faceX, const Face<T> &faceY);

#include "../src/Face.cpp"

struct FaceHash
{
    template <typename T>
    size_t operator()(const Face<T> &face) const
    {
        std::hash<int> hasher;
        size_t seed = 0;
        const auto v = face.getVertices();
        for (const int &i : v)
        {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

#endif // __FACE_H