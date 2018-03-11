#pragma once

#include <pods/pods.h>

struct B1
{
    int a = 10;

    PODS_SERIALIZABLE(1, PODS_MDR(a))
};

struct B2
{
    int a = 11;
    int b = 12;

    static constexpr pods::Version version()
    {
        return 2;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version version)
    {
        if (version == 1)
        {
            return serializer(PODS_MDR(a));
        }

        return serializer(PODS_MDR(a), PODS_MDR(b));
    }
};

struct A1
{
    int x = 1;

    B1 b;

    PODS_SERIALIZABLE(1, PODS_MDR(x), PODS_MDR(b))
};

struct A2
{
    int x = 3;
    int y = 5;

    B1 b;

    static constexpr pods::Version version()
    {
        return 2;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version version)
    {
        if (version == 1)
        {
            return serializer(PODS_MDR(x), PODS_MDR(b));
        }

        return serializer(PODS_MDR(x), PODS_MDR(y), PODS_MDR(b));
    }
};

struct A3
{
    int x = 7;
    int y = 9;

    B2 b;

    static constexpr pods::Version version()
    {
        return 2;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version version)
    {
        if (version == 1)
        {
            return serializer(PODS_MDR(x), PODS_MDR(b));
        }

        return serializer(PODS_MDR(x), PODS_MDR(y), PODS_MDR(b));
    }
};
