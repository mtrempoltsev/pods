#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

#include <pods/pods.h>

enum Enum1
{
    AA,
    BB
};

enum class Enum2 : unsigned char
{
    X,
    Y
};

struct A
{
    A() = default;

    A(const A&) = delete;
    A& operator=(const A&) = delete;

    A(A&&) = delete;
    A& operator=(A&&) = delete;

    std::array<float, 5> floatData = { 1, 2, 3, 4, 5 };

    static constexpr pods::Version version()
    {
        return 5;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version archiveVersion)
    {
        return serializer
        (
            PODS_OPT(floatData)
        );
    }
};

struct TestData
{
    int x = 100;
    double y = 200.5;
    std::string z = "hello!";

    A a;

    struct B
    {
        struct C
        {
            bool b = false;
            Enum1 e1 = ::BB;
            Enum2 e2 = Enum2::X;

            static constexpr pods::Version version()
            {
                return 1;
            }

            template <class Serializer>
            pods::Error serialize(Serializer& serializer, pods::Version archiveVersion)
            {
                return serializer
                (
                    PODS_OPT(b),
                    PODS_MDR(e1),
                    PODS_MDR(e2)
                );
            }
        };

        C c;

        static constexpr pods::Version version()
        {
            return 2;
        }

        template <class Serializer>
        pods::Error serialize(Serializer& serializer, pods::Version archiveVersion)
        {
            return serializer
            (
                PODS_MDR(c)
            );
        }
    };

    B b;

    struct Point
    {
        int x;
        int y;

        bool operator==(const Point other) const
        {
            return x == other.x && y == other.y;
        }

        static constexpr pods::Version version()
        {
            return 1;
        }

        template <class Serializer>
        pods::Error serialize(Serializer& serializer, pods::Version archiveVersion)
        {
            return serializer
            (
                PODS_MDR(x),
                PODS_OPT(y)
            );
        }
    };

    std::vector<Point> points = { { 1, 2}, { 3, 4 } };

    std::map<short, std::vector<char>> dict = { { 1, { 'a', 'b' } }, { -5, {} } };

    static constexpr pods::Version version()
    {
        return 1;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version archiveVersion)
    {
        return serializer
        (
            PODS_MDR(x),
            PODS_OPT(y),
            PODS_MDR(z),
            PODS_OPT(a),
            PODS_MDR(b),
            PODS_OPT(points),
            PODS_MDR(dict)
        );
    }
};
