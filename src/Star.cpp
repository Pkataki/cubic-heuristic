template <typename T>
Star<T>::Star(Face<T> i, Face<T> j, Face<T> k, Face<T> l)
{
    this->i = i;
    std::vector<Face<T>> sortedFaces{j, k, l};
    std::sort(sortedFaces.begin(), sortedFaces.end());
    this->j = sortedFaces[0];
    this->k = sortedFaces[1];
    this->l = sortedFaces[2];
    buildCycle();
}

template <typename T>
Star<T>::Star(const std::vector<Face<T>> &faces)
{
    this->i = faces[0];

    std::vector<Face<T>> sortedFaces{faces[1], faces[2], faces[3]};
    std::sort(sortedFaces.begin(), sortedFaces.end());
    this->j = sortedFaces[0];
    this->k = sortedFaces[1];
    this->l = sortedFaces[2];

    buildCycle();
}

template <typename T>
T Star<T>::getCost() const
{
    return i.getCost() + j.getCost() + k.getCost() + l.getCost();
}

template <typename T>
std::vector<Face<T>> Star<T>::getFaces() const
{
    std::vector<Face<T>> faces{i, j, k, l};
    return faces;
}

template <typename T>
std::vector<std::pair<int,int> > Star<T>::getEdges() const
{
    std::set<std::pair<int,int> >setEdges;
    for(const auto &face : getFaces())
        for(const auto &edge : face.getEdges())
            setEdges.insert(edge);
    return std::vector<std::pair<int,int> >(setEdges.begin(), setEdges.end());
}

template <typename T>
std::vector<Face<T>> Star<T>::getFacesWithoutCenter() const
{
    std::vector<Face<T>> faces{j, k, l};
    return faces;
}

template <typename T>
Face<T> Star<T>::getCenter() const
{
    return i;
}

template <typename T>
std::vector<int> Star<T>::getVertices() const
{
    return vertices;
}

template <typename T>
void Star<T>::buildCycle()
{
    for (auto &face : getFacesWithoutCenter())
    {
        auto verticesFace = face.getVertices();
        for (auto &vertex : verticesFace)
        {
            vertices.push_back(vertex);
        }

        // create list of vertices of the cycle
        for (auto &vertex : verticesFace)
        {
            vertices.push_back(vertex);
        }

        uniqueVector(vertices);

        // create adjancency list of the cycle
        for (auto &u : verticesFace)
        {
            for (auto &v : verticesFace)
            {
                if (u != v and not i.hasEdge(u, v))
                {
                    adj[u].push_back(v);
                }
            }
        }
    }

    for (auto &&[vertex, list] : adj)
    {
        uniqueVector(list);
    }
}

template <typename T>
std::vector<int> Star<T>::getCycle() const
{

    std::set<int> seen;
    std::vector<int> cycle;

    auto findCenter = [=]() -> int {
        for (auto &&[vertex, list] : adj)
        {
            if (list.size() == 1)
                return vertex;
        }
        return -1;
    };

    bool canGoForward = true;
    int curVertex = vertices.size() == 6 ? *vertices.begin() : findCenter();
    seen.insert(curVertex);

    while (canGoForward)
    {
        canGoForward = false;
        cycle.push_back(curVertex);
        for (auto &nextVertex : adj.at(curVertex))
        {
            if (!hasValueSet(seen, nextVertex))
            {
                seen.insert(nextVertex);
                curVertex = nextVertex;
                canGoForward = true;
                break;
            }
        }
    }

    if (cycle.size() == 5)
        std::rotate(cycle.begin(), cycle.begin() + 1, cycle.end());

    return cycle;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, Star<T> &star)
{
    auto faces = star.getFaces();
    out << "Face i: " << faces[0] << std::endl;
    out << "Face j: " << faces[1] << std::endl;
    out << "Face k: " << faces[2] << std::endl;
    out << "Face l: " << faces[3];
    return out;
}

template <typename T>
bool operator==(const Star<T> &starX, const Star<T> &starY)
{
    auto vectorFacesStarX = starX.getFaces();
    auto vectorFacesStarY = starY.getFaces();
    return vectorFacesStarX == vectorFacesStarY;
}

template <typename T>
bool operator!=(const Star<T> starX, const Star<T> starY)
{
    return !(starX == starY);
}

template <typename T>
bool operator<(const Star<T> starX, const Star<T> starY)
{
    return starX.getCost() < starY.getCost();
}
