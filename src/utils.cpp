#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

void uniqueVector(std::vector<int> &v)
{
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
}

bool hasValue(std::vector<int> &v, int value)
{
    return std::find(v.begin(), v.end(), value) != v.end();
}

bool hasValueSet(std::set<int> &s, int value)
{
    return s.find(value) != s.end();
}

const double EPS = 1e-9;

int dblCmp(double a, double b)
{
    if (fabs(a - b) < EPS)
        return 0;
    return a > b ? 1 : -1;
}
