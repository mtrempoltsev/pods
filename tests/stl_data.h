#pragma once

#include <memory>
#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

#include <pods/pods.h>

struct In
{
    int k;

    bool operator==(const In& other) const
    {
        return k == other.k;
    }

    bool operator!=(const In& other) const
    {
        return !(*this == other);
    }

    PODS_SERIALIZABLE(1, PODS_MDR(k))
};

struct StlArray
{
    std::array<int, 3> x = { { 1, 2, 3 } };
    std::array<In, 3> y = { { { 4 }, { 5 }, { 6 } } };
    std::array<std::array<int, 1>, 1> z = { { { { 7 } } } };
    PODS_SERIALIZABLE(1, PODS_MDR(x), PODS_MDR(y), PODS_MDR(z))
};

struct StlVector
{
    std::vector<int> x = { 1, 2, 3 };
    std::vector<In> y = { { { 4 }, { 5 }, { 6 } } };
    std::vector<std::vector<int>> z = { { 7 } };
    PODS_SERIALIZABLE(1, PODS_MDR(x), PODS_MDR(y), PODS_MDR(z))
};

struct StlDeque
{
    std::deque<int> x = { 1, 2, 3 };
    std::deque<In> y = { { { 4 }, { 5 }, { 6 } } };
    std::deque<std::deque<int>> z = { { 7 } };
    PODS_SERIALIZABLE(1, PODS_MDR(x), PODS_MDR(y), PODS_MDR(z))
};

struct StlList
{
    std::list<int> x = { 1, 2, 3 };
    std::list<In> y = { { { 4 }, { 5 }, { 6 } } };
    std::list<std::list<int>> z = { { 7 } };
    PODS_SERIALIZABLE(1, PODS_MDR(x), PODS_MDR(y), PODS_MDR(z))
};

struct StlForwardList
{
    std::forward_list<int> x = { 1, 2, 3 };
    std::forward_list<In> y = { { { 4 },{ 5 },{ 6 } } };
    std::forward_list<std::list<int>> z = { { 7 } };
    PODS_SERIALIZABLE(1, PODS_MDR(x), PODS_MDR(y), PODS_MDR(z))
};

void clear(StlArray& data)
{
    std::fill(data.x.begin(), data.x.end(), 0);
    std::fill(data.y.begin(), data.y.end(), In { 0 } );
    std::fill(data.z.begin(), data.z.end(), std::array<int, 1> { { 0 } });
}

template <class T>
void clear(T& data)
{
    data.x.clear();
    data.y.clear();
    data.z.clear();
}
