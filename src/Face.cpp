
template <typename T>
Face<T>::Face()
{
    cost = 0;
}

template <typename T>
Face<T>::Face(std::vector<int> vertices, T cost)
{
    std::sort(vertices.begin(), vertices.end());

    this->a = vertices[0];
    this->b = vertices[1];
    this->c = vertices[2];

    this->cost = cost;
}

template <typename T>
Face<T>::Face(int a, int b, int c, T cost)
{
    std::vector<int> tmpVertices{a, b, c};

    std::sort(tmpVertices.begin(), tmpVertices.end());

    this->a = tmpVertices[0];
    this->b = tmpVertices[1];
    this->c = tmpVertices[2];

    this->cost = cost;
}

template <typename T>
T Face<T>::getCost() const
{
    return cost;
}

template <typename T>
std::vector<std::pair<int,int> > Face<T>::getEdges() const
{
    std::vector<std::pair<int, int>> edges;
    auto vertices = getVertices();
    edges.push_back({vertices[0], vertices[1]});
    edges.push_back({vertices[0], vertices[2]});
    edges.push_back({vertices[1], vertices[2]});
    std::sort(edges.begin(), edges.end());
    return edges;
}

template <typename T>
void Face<T>::setCost(T cost)
{
    this->cost = cost;
}

template <typename T>
std::vector<int> Face<T>::getVertices() const
{
    std::vector<int> vertices{a, b, c};
    return vertices;
}

template <typename T>
bool Face<T>::hasEdge(int u, int v) const
{
    auto vertices = getVertices();
    return hasValue(vertices, u) && hasValue(vertices, v);
}

template <typename T>
bool Face<T>::isAdjacent(Face<T> otherFace) const
{
    auto vertices = getVertices();
    for (auto &u : vertices)
    {
        for (auto &v : vertices)
        {
            if (u != v && otherFace.hasEdge(u, v))
                return true;
        }
    }
    return false;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, Face<T> face)
{
    auto vertices = face.getVertices();
    out << "a: " << vertices[0];
    out << " b: " << vertices[1];
    out << " c: " << vertices[2];
    return out;
}

template <typename T>
bool operator==(const Face<T> &faceX, const Face<T> &faceY)
{
    return faceX.getVertices() == faceY.getVertices();
}

template <typename T>
bool operator!=(const Face<T> &faceX, const Face<T> &faceY)
{
    return !(faceX == faceY);
}

template <typename T>
bool operator<(const Face<T> &faceX, const Face<T> &faceY)
{
    return faceX.getCost() < faceY.getCost();
}
