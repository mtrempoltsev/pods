#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

#include <serialization++/serialization++.h>

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

    static constexpr spp::Version version()
    {
        return 5;
    }

    template <class Serializer>
    spp::Error serialize(Serializer& serializer, spp::Version archiveVersion)
    {
        return serializer
        (
            SPP_OPT(floatData)
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

            static constexpr spp::Version version()
            {
                return 1;
            }

            template <class Serializer>
            spp::Error serialize(Serializer& serializer, spp::Version archiveVersion)
            {
                return serializer
                (
                    SPP_OPT(b),
                    SPP_MDR(e1),
                    SPP_MDR(e2)
                );
            }
        };

        C c;

        static constexpr spp::Version version()
        {
            return 2;
        }

        template <class Serializer>
        spp::Error serialize(Serializer& serializer, spp::Version archiveVersion)
        {
            return serializer
            (
                SPP_MDR(c)
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

        static constexpr spp::Version version()
        {
            return 1;
        }

        template <class Serializer>
        spp::Error serialize(Serializer& serializer, spp::Version archiveVersion)
        {
            return serializer
            (
                SPP_MDR(x),
                SPP_OPT(y)
            );
        }
    };

    std::vector<Point> points = { { 1, 2}, { 3, 4 } };

    std::map<short, std::vector<char>> dict = { { 1, { 'a', 'b' } }, { -5, {} } };

    static constexpr spp::Version version()
    {
        return 1;
    }

    template <class Serializer>
    spp::Error serialize(Serializer& serializer, spp::Version archiveVersion)
    {
        return serializer
        (
            SPP_MDR(x),
            SPP_OPT(y),
            SPP_MDR(z),
            SPP_OPT(a),
            SPP_MDR(b),
            SPP_OPT(points),
            SPP_MDR(dict)
        );
    }
};
