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

    bool operator<(const In& other) const
    {
        return k < other.k;
    }

    PODS_SERIALIZABLE(1, PODS_MDR(k))
};

struct StlMap
{
    std::map<int, int> a = { { 1, 2}, { 3, 4 } };
    std::map<In, int> b = { { { 1 }, 2 } };
    std::map<int, In> c = { { 1, { 2 } } };
    std::map<In, In> d = { { { 1 }, { 2 } } };
    std::map<std::map<int, int>, std::map<int, int>> e = { { { { 1, 2 } }, { { 3, 4 } } } };

    PODS_SERIALIZABLE(1,
        PODS_MDR(a),
        PODS_MDR(b),
        PODS_MDR(c),
        PODS_MDR(d),
        PODS_MDR(e))
};

template <class T>
void clear(T& data)
{
    data.a.clear();
    data.b.clear();
    data.c.clear();
    data.d.clear();
    data.e.clear();
}
