template <typename T>
DualPlanarGraph<T>::DualPlanarGraph(const std::vector<std::vector<T>> &costs,
                                    const std::vector<std::vector<int>> &faces)
{
    // build the faces vector
    this->costs = costs;
    for (const auto &face : faces)
        this->faces.push_back(makeFace(face));

    this->stars = buildStars(this->faces);

    indexStar.clear();

    for (int i = 0; i < stars.size(); i++)
        indexStar[stars[i].getCenter()] = i;

    for (const auto &edge : getEdges())
        edgesPrimalGraph.insert(edge);
}

template <typename T>
int DualPlanarGraph<T>::getSize() const
{
    return stars.size();
}

template <typename T>
std::vector<std::vector<int>> DualPlanarGraph<T>::convertFacesToVector(const std::vector<Face<T>> &faces)
{
    std::vector<std::vector<int>> faceList;
    for (auto &face : faces)
        faceList.push_back(face.getVertices());

    return faceList;
}

template <typename T>
std::vector<std::pair<int, int>> DualPlanarGraph<T>::getEdges() const
{
    std::set<std::pair<int, int>> edgesSet;
    for (auto &star : stars)
    {
        auto faces = star.getFaces();
        for (auto &face : faces)
        {
            auto vertices = face.getVertices();
            std::sort(vertices.begin(), vertices.end());
            edgesSet.insert({vertices[0], vertices[1]});
            edgesSet.insert({vertices[0], vertices[2]});
            edgesSet.insert({vertices[1], vertices[2]});
        }
    }
    std::vector<std::pair<int, int>> edgesVector;
    for (auto &edge : edgesSet)
        edgesVector.push_back(edge);
    return edgesVector;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::getStars() const
{
    return stars;
}

template <typename T>
T DualPlanarGraph<T>::getCost() const
{
    T cost = 0;
    for (const auto &star : stars)
        cost += star.getCenter().getCost();

    return cost / 2;
}

template <typename T>
DualPlanarGraph<T> DualPlanarGraph<T>::buildDualPlanarSolution(const std::vector<Face<T>> &faces)
{
    DualPlanarGraph neighbor = DualPlanarGraph(this->costs, convertFacesToVector(faces));
    return neighbor;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::buildStars(const std::vector<Face<T>> &faces)
{
    std::vector<Star<T>> stars;
    for (auto &faceX : faces)
    {
        std::vector<Face<T>> adjacentsToFaceX{faceX};
        for (auto &faceY : faces)
            if (faceX != faceY && faceX.isAdjacent(faceY))
                adjacentsToFaceX.push_back(faceY);

        if (adjacentsToFaceX.size() == 4)
            stars.push_back(Star<T>(adjacentsToFaceX));
        else
        {
            std::cout << "ON buildStars: " << faceX << std::endl;
            abort();
        }
    }
    return stars;
}

template <typename T>
Face<T> DualPlanarGraph<T>::makeFace(const std::vector<int> &face)
{

    T cost = 0;
    for (int i = 0; i < face.size(); i++)
        for (int j = i + 1; j < face.size(); j++)
            cost += costs[face[i]][face[j]];

    Face<T> newFace(face, cost);
    return newFace;
}

//Case when cycle has five vertices

template <typename T>
Star<T> DualPlanarGraph<T>::generateRegular5Star(const Star<T> &star)
{
    auto cycle = star.getCycle();

    //the last vertex on the cycle is the center
    int n = cycle.size() - 1;
    int center = cycle.back();

    std::vector<Face<T>> newFaces;

    for (int i = 0; i < n; i++)
    {
        int curVertex = cycle[i];
        int nextVertex = cycle[(i + 1) % n];
        newFaces.push_back(makeFace({center, curVertex, nextVertex}));
    }

    Star<T> newStar(newFaces);

    return newStar;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::generate5StarsByRotation(const Star<T> &star)
{

    std::vector<Star<T>> starsList;

    auto cycle = star.getCycle();
    //the last vertex on the cycle is the center
    int n = cycle.size() - 1;
    int center = cycle.back();

    auto generate5StarRotatedOn = [&](int indexOnCycle) -> Star<T> {
        std::vector<Face<T>> newFaces;
        std::vector<std::vector<int>> newFacesVertices;

        int curVertex = cycle[indexOnCycle];
        int nextVertex = cycle[(indexOnCycle + 1) % n];
        int prevVertex = cycle[(indexOnCycle - 1 + n) % n];
        int nextNextVertex = cycle[(indexOnCycle + 2) % n];

        newFaces.push_back(makeFace({center, prevVertex, nextVertex}));
        newFaces.push_back(makeFace({center, curVertex, nextVertex}));
        newFaces.push_back(makeFace({center, prevVertex, curVertex}));
        newFaces.push_back(makeFace({nextNextVertex, prevVertex, nextVertex}));

        Star<T> newStar(newFaces);

        return newStar;
    };

    for (int i = 0; i < cycle.size() - 1; i++)
        starsList.push_back(generate5StarRotatedOn(i));

    return starsList;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::generate6StarsType1(const Star<T> &star)
{
    std::vector<Star<T>> starsList;

    auto cycle = star.getCycle();
    int n = cycle.size();

    auto generateStarType1RotateOn = [&](int indexOnCycle) -> Star<T> {
        std::vector<Face<T>> newFaces;
        int curVertex = cycle[indexOnCycle];
        int nextNextVertex = cycle[(indexOnCycle + 2) % n];
        int prevPrevVertex = cycle[(indexOnCycle - 2 + n) % n];
        newFaces.push_back(makeFace({curVertex, prevPrevVertex, nextNextVertex}));

        for (int counter = 1; counter <= 3; counter++)
        {
            curVertex = cycle[indexOnCycle];
            int nextVertex = cycle[(indexOnCycle + 1) % n];
            nextNextVertex = cycle[(indexOnCycle + 2) % n];

            newFaces.push_back(makeFace({curVertex, nextVertex, nextNextVertex}));
            indexOnCycle = (indexOnCycle + 2) % n;
        }

        Star<T> newStar(newFaces);

        return newStar;
    };

    for (int i = 0; i < 2; i++)
        starsList.push_back(generateStarType1RotateOn(i));

    return starsList;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::generate6StarsType2(const Star<T> &star)
{
    std::vector<Star<T>> starsList;
    auto cycle = star.getCycle();
    int n = cycle.size();

    auto generateStarType2RotateOn = [&](const int indexOnCycle) {
        std::vector<Face<T>> newFaces;

        int curVertex = cycle[indexOnCycle];
        int nextVertex = cycle[(indexOnCycle + 1) % n];
        int nextNextVertex = cycle[(indexOnCycle + 2) % n];
        int nextNextNextVertex = cycle[(indexOnCycle + 3) % n];
        int prevVertex = cycle[(indexOnCycle - 1 + n) % n];
        int prevPrevVertex = cycle[(indexOnCycle - 2 + n) % n];
        int prevPrevPrevVertex = cycle[(indexOnCycle - 3 + n) % n];

        newFaces.push_back(makeFace({curVertex, prevPrevVertex, prevVertex}));
        newFaces.push_back(makeFace({curVertex, prevPrevVertex, nextVertex}));
        newFaces.push_back(makeFace({nextVertex, nextNextVertex, nextNextNextVertex}));
        newFaces.push_back(makeFace({nextVertex, prevPrevVertex, nextNextNextVertex}));

        Star<T> newStar(newFaces);

        return newStar;
    };

    for (int i = 0; i < 3; i++)
        starsList.push_back(generateStarType2RotateOn(i));

    return starsList;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::generate6StarsType3(const Star<T> &star)
{
    std::vector<Star<T>> starsList;
    auto cycle = star.getCycle();
    int n = cycle.size();

    auto generateStarType2RotateOn = [&](const int indexOnCycle) {
        std::vector<Face<T>> newFaces;

        int curVertex = cycle[indexOnCycle];
        int nextVertex = cycle[(indexOnCycle + 1) % n];
        int nextNextVertex = cycle[(indexOnCycle + 2) % n];
        int nextNextNextVertex = cycle[(indexOnCycle + 3) % n];
        int prevVertex = cycle[(indexOnCycle - 1 + n) % n];
        int prevPrevVertex = cycle[(indexOnCycle - 2 + n) % n];
        int prevPrevPrevVertex = cycle[(indexOnCycle - 3 + n) % n];

        newFaces.push_back(makeFace({curVertex, prevPrevVertex, prevVertex}));
        newFaces.push_back(makeFace({curVertex, nextVertex, nextNextNextVertex}));
        newFaces.push_back(makeFace({nextVertex, nextNextVertex, nextNextNextVertex}));
        newFaces.push_back(makeFace({curVertex, prevPrevVertex, nextNextNextVertex}));

        Star<T> newStar(newFaces);

        return newStar;
    };

    for (int i = 0; i < 3; i++)
        starsList.push_back(generateStarType2RotateOn(i));

    return starsList;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::generate6StarsType4(const Star<T> &star)
{
    std::vector<Star<T>> starsList;

    auto cycle = star.getCycle();
    int n = cycle.size();

    auto generateStarType3RotateOn = [&](int indexOnCycle) {
        std::vector<Face<T>> newFaces;
        int curVertex = cycle[indexOnCycle];

        for (int counter = 1; counter <= 4; counter++)
        {
            int nextVertex = cycle[(indexOnCycle + 1) % n];
            int nextNextVertex = cycle[(indexOnCycle + 2) % n];

            newFaces.push_back(makeFace({curVertex, nextVertex, nextNextVertex}));
            indexOnCycle = (indexOnCycle + 1) % n;
        }

        Star<T> newStar(newFaces);

        return newStar;
    };

    for (int i = 0; i < n; i++)
    {
        starsList.push_back(generateStarType3RotateOn(i));
    }

    return starsList;
}

template <typename T>
std::vector<Star<T>> DualPlanarGraph<T>::generateAllPossibleStarsOnRemoving(const Star<T> &star)
{
    std::vector<Star<T>> allPossibleStars;
    auto cycle = star.getCycle();

    if (cycle.size() == 5)
    {
        allPossibleStars.push_back(generateRegular5Star(star));
        merge(allPossibleStars, generate5StarsByRotation(star));
    }
    else
    {
        merge(allPossibleStars, generate6StarsType1(star));
        merge(allPossibleStars, generate6StarsType2(star));
        merge(allPossibleStars, generate6StarsType3(star));
        merge(allPossibleStars, generate6StarsType4(star));
    }
    return allPossibleStars;
}

template <typename T>
bool DualPlanarGraph<T>::checkIfCanRebuildDualPlanar(const Star<T> &starToBeRemoved,
                                                     const Star<T> &starToBeInserted)
{
    const auto facesToBeRemoved = starToBeRemoved.getFaces();
    const auto facesToBeInserted = starToBeInserted.getFaces();

    std::unordered_set<std::pair<int, int>, PairHash> setEdgesFaceToBeRemoved;
    std::unordered_set<std::pair<int, int>, PairHash> setEdgesFaceToBeInserted;

    for (const auto &face : facesToBeRemoved)
    {
        for (const auto &edge : face.getEdges())
        {
            setEdgesFaceToBeRemoved.insert(edge);
        }
    }

    for (const auto &face : facesToBeInserted)
    {
        for (const auto &edge : face.getEdges())
        {
            setEdgesFaceToBeInserted.insert(edge);
        }
    }

    for (const auto &edge : setEdgesFaceToBeInserted)
    {
        if (edgesPrimalGraph.count(edge))
        {
            if (!setEdgesFaceToBeRemoved.count(edge))
            {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
DualPlanarGraph<T> DualPlanarGraph<T>::rebuildDualPlanar(const Star<T> &starToBeRemoved,
                                                         const Star<T> &starToBeInserted)
{
    const auto facesToBeRemoved = starToBeRemoved.getFaces();
    const auto facesToBeInserted = starToBeInserted.getFaces();
    // auto newFaces = faces;

    std::vector<Face<T>> newFaces;

    for (const auto &face : faces)
    {
        bool canPut = true;
        for (const auto &faceToRemove : facesToBeRemoved)
            canPut &= faceToRemove != face;
        if (canPut)
            newFaces.push_back(face);
    }

    for (const auto &face : facesToBeInserted)
        newFaces.push_back(face);
    
    return buildDualPlanarSolution(newFaces);
}

template <typename T>
std::vector<DualPlanarGraph<T>> DualPlanarGraph<T>::getNeighborSolutions()
{
    std::vector<DualPlanarGraph<T>> neighborSolutions;
    for (auto &star : stars)
    {
        auto allPossibleStars = generateAllPossibleStarsOnRemoving(star);
        for (auto &newStar : allPossibleStars)
        {
            if (newStar != star)
            {
                bool canRebuild = checkIfCanRebuildDualPlanar(star, newStar);
                if (canRebuild)
                {
                    auto newSolution = rebuildDualPlanar(star, newStar);
                    neighborSolutions.push_back(newSolution);
                }
            }
        }
    }
    return neighborSolutions;
}

template <typename T>
DualPlanarGraph<T> DualPlanarGraph<T>::getRandomNeighbor()
{

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, stars.size() - 1);

    std::vector<std::pair<Star<T>, Star<T>>> starsNeighbors;

    while (starsNeighbors.empty())
    {
        auto randomIndex = dist(rng);
        auto randomStar = stars[randomIndex];
        auto allPossibleStars = generateAllPossibleStarsOnRemoving(randomStar);

        for (auto &newStar : allPossibleStars)
        {
            if (newStar != randomStar)
            {
                auto canRebuild = checkIfCanRebuildDualPlanar(randomStar, newStar);
                if (canRebuild)
                    starsNeighbors.push_back({randomStar, newStar});
            }
        }
    }

    std::uniform_int_distribution<int> dist1(0, starsNeighbors.size() - 1);
    auto randomNeighborIdx = dist1(rng);
    auto randomStars = starsNeighbors[randomNeighborIdx];

    return rebuildDualPlanar(randomStars.first, randomStars.second);
}

template <typename T>
std::vector<DualPlanarGraph<T>> DualPlanarGraph<T>::getBestKNeighbors(int k)
{
    std::vector<std::pair<Star<T>, Star<T>>> neighborStars = {};
    std::vector<DualPlanarGraph<T>> bestNeighbors;
    for (const auto &star : stars)
    {
        auto allPossibleStars = generateAllPossibleStarsOnRemoving(star);
        for (auto &newStar : allPossibleStars)
        {
            if (newStar != star)
            {
                auto canRebuild = checkIfCanRebuildDualPlanar(star, newStar);
                if (canRebuild)
                    neighborStars.push_back({star, newStar});
            }
        }
    }

    auto getCostPairQuadFaces = [&](const std::pair<Star<T>, Star<T>> &pair) {
        auto cycle = pair.first.getCycle();
        T costCycle = 0;
        for (int i = 0; i < cycle.size(); i++)
            costCycle += costs[cycle[i]][cycle[(i + 1) % cycle.size()]];
        T costFirst = pair.first.getCost() - costCycle;
        costFirst /= 2;
        costFirst += costCycle;

        T costSecond = pair.second.getCost() - costCycle;
        costSecond /= 2;
        costSecond += costCycle;

        return costSecond - costFirst;
    };

    // std::sort(neighborStars.begin(), neighborStars.end(),
    //           [](const std::pair<Star<T>, Star<T>> &a, const std::pair<Star<T>, Star<T>> &b) {
    //               return (a.second.getCost() - a.first.getCost()) >
    //                      (b.second.getCost() - b.first.getCost());
    //           });

    std::sort(neighborStars.begin(), neighborStars.end(),
              [&](const std::pair<Star<T>, Star<T>> &a, const std::pair<Star<T>, Star<T>> &b) {
                  return getCostPairQuadFaces(a) >
                         getCostPairQuadFaces(b);
              });

    for (const auto &starPair : neighborStars)
    {
        bool canRebuild = true;
        auto newSolution = rebuildDualPlanar(starPair.first, starPair.second);
        bestNeighbors.push_back(newSolution);
        if (bestNeighbors.size() == k)
            break;
    }

    // std::cout << "\n\n\n";

    return bestNeighbors;
}

template <typename T>
std::vector<DualPlanarGraph<T>> DualPlanarGraph<T>::getBestKNeighbors(const std::map<std::vector<std::pair<int, int>>, int> &tabuTable, int k)
{
    std::vector<std::pair<Star<T>, Star<T>>> neighborStars = {};
    std::vector<DualPlanarGraph<T>> bestNeighbors;
    for (const auto &star : stars)
    {
        auto allPossibleStars = generateAllPossibleStarsOnRemoving(star);
        for (auto &newStar : allPossibleStars)
        {
            if (newStar != star)
            {
                auto canRebuild = checkIfCanRebuildDualPlanar(star, newStar);
                if (canRebuild)
                    neighborStars.push_back({star, newStar});
            }
        }
    }

    auto getCostPairQuadFaces = [&](const std::pair<Star<T>, Star<T>> &pair) {
        auto cycle = pair.first.getCycle();
        T costCycle = 0;
        for (int i = 0; i < cycle.size(); i++)
            costCycle += costs[cycle[i]][cycle[(i + 1) % cycle.size()]];
        T costFirst = pair.first.getCost() - costCycle;
        costFirst /= 2;
        costFirst += costCycle;

        T costSecond = pair.second.getCost() - costCycle;
        costSecond /= 2;
        costSecond += costCycle;

        return costSecond - costFirst;
    };

    std::sort(neighborStars.begin(), neighborStars.end(),
              [&](const std::pair<Star<T>, Star<T>> &a, const std::pair<Star<T>, Star<T>> &b) {
                  return getCostPairQuadFaces(a) >
                         getCostPairQuadFaces(b);
              });

    for (const auto &starPair : neighborStars)
    {
        auto newSolution = rebuildDualPlanar(starPair.first, starPair.second);

        if (!tabuTable.count(newSolution.getEdges()))
            bestNeighbors.push_back(newSolution);

        if (bestNeighbors.size() == k)
            break;
    }

    return bestNeighbors;
}

template <typename T>
std::vector<DualPlanarGraph<T>> DualPlanarGraph<T>::getCustomBestKNeighbors(std::map<std::vector<std::pair<int, int>>, int> &tabuTable, int k, const int &iteration)
{
    std::vector<std::pair<Star<T>, Star<T>>> neighborStars = {};
    std::vector<DualPlanarGraph<T>> bestNeighbors;
    for (const auto &star : stars)
    {
        auto allPossibleStars = generateAllPossibleStarsOnRemoving(star);
        for (auto &newStar : allPossibleStars)
        {
            if (newStar != star)
            {
                auto canRebuild = checkIfCanRebuildDualPlanar(star, newStar);
                if (canRebuild)
                    neighborStars.push_back({star, newStar});
            }
        }
    }

    auto getCostPairQuadFaces = [&](const std::pair<Star<T>, Star<T>> &pair) 
    {
        auto cycle = pair.first.getCycle();
        T costCycle = 0;
        for (int i = 0; i < cycle.size(); i++)
            costCycle += costs[cycle[i]][cycle[(i + 1) % cycle.size()]];
        T costFirst = pair.first.getCost() - costCycle;
        costFirst /= 2;
        costFirst += costCycle;

        T costSecond = pair.second.getCost() - costCycle;
        costSecond /= 2;
        costSecond += costCycle;

        return costSecond - costFirst;
    };

    std::sort(neighborStars.begin(), neighborStars.end(),
              [&](const std::pair<Star<T>, Star<T>> &a, const std::pair<Star<T>, Star<T>> &b) {
                  return getCostPairQuadFaces(a) >
                         getCostPairQuadFaces(b);
              });

    for (const auto &starPair : neighborStars)
    {
        if (!tabuTable.count(starPair.second.getEdges()) && bestNeighbors.size() < k)
        {
            auto newSolution = rebuildDualPlanar(starPair.first, starPair.second);
            tabuTable[starPair.second.getEdges()] = iteration;
            bestNeighbors.push_back(newSolution);
        }
        
        if (getCostPairQuadFaces(starPair) < 0)
            tabuTable[starPair.second.getEdges()] = iteration;
    }

    return bestNeighbors;
}

template <typename T>
bool operator<(const DualPlanarGraph<T> &a, const DualPlanarGraph<T> &b)
{
    return a.getCost() < b.getCost();
}


template <typename T>
bool operator==(const DualPlanarGraph<T> &a, const DualPlanarGraph<T> &b)
{
    return a.getEdges() == b.getEdges();
}