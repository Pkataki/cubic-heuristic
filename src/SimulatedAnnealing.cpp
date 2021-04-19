
template <typename T>
SimulatedAnnealing<T>::SimulatedAnnealing() {}

template <typename T>
SimulatedAnnealing<T>::~SimulatedAnnealing() {}

template <typename T>
SimulatedAnnealing<T>::SimulatedAnnealing(const std::vector<std::vector<T>> &costs,
                                          const std::vector<std::vector<int>> &faces,
                                          double temperature,
                                          double alpha,
                                          int numIterations,
                                          int numDisturbance,
                                          int maxNumSuccess)
{
    this->dualPlanarGraph = DualPlanarGraph(costs, faces);
    this->temperature = temperature;
    this->alpha = alpha;
    this->numIterations = numIterations;
    this->numDisturbance = numDisturbance;
    this->maxNumSuccess = maxNumSuccess;
}

int getRandomInterval(int l, int r)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(l, r);
    return dist(mt);
}

template <typename T>
DualPlanarGraph<T> SimulatedAnnealing<T>::disturb(DualPlanarGraph<T> &curSolution)
{
    auto neighborSolutions = curSolution.getBestKNeighbors(3);
    // std::sort(neighborSolutions.begin(), neighborSolutions.end(),
    //     [](DualPlanarGraph<T> a, DualPlanarGraph<T> b){
    //         return a.getCost() > b.getCost();
    // });
    int n                  = neighborSolutions.size();
    int randomIndex        = getRandomInterval(0, n-1);
    return neighborSolutions[randomIndex];
    // auto randomNeighbor = curSolution;
    // int k = getRandomInterval(1, 3);
    // if (getRandomInterval(1, 100) <= 15)
    //     k = getRandomInterval(5, 10);
    // while (k--)
    //     randomNeighbor = randomNeighbor.getRandomNeighbor();

    // return randomNeighbor;
}

double getRandom()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(mt);
}

template <typename T>
DualPlanarGraph<T> SimulatedAnnealing<T>::getBestSolution()
{
    auto curSolution = this->dualPlanarGraph;
    int iteration = 1;
    int numSuccess = 0;

    std::vector<std::pair<int, int>> points;
    do
    {
        numSuccess = 0;
        int disturbance = 0;
        do
        {
            auto neighborSolution = disturb(curSolution);
            T f = curSolution.getCost() - neighborSolution.getCost();

            std::cout << curSolution.getCost() << ' ' << neighborSolution.getCost() << std::endl;

            double expValue = exp(-f / temperature);
            double randomValue = getRandom();

            std::cout << std::fixed << std::setprecision(10) << expValue << ' ' << randomValue << ' ' << temperature << std::endl;

            if (f <= 0 || dblCmp(expValue, randomValue) > 0)
            {
                numSuccess++;
                curSolution = neighborSolution;
            }
            disturbance++;

        } while (numSuccess <= maxNumSuccess && disturbance <= numDisturbance && dblCmp(temperature, 0));
        points.push_back({curSolution.getCost(), iteration});
        temperature *= alpha;
        iteration++;

    } while (numSuccess != 0 && iteration <= numIterations && dblCmp(temperature, 0));

    for (auto &p : points)
        std::cout << p.first << ' ' << p.second << std::endl;

    std::cout << curSolution.getCost() << std::endl;
    return curSolution;
}