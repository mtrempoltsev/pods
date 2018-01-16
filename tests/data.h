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
    std::array<float, 5> floatData = { 1, 2, 3, 4, 5 };

    constexpr spp::Version actualDataVersion() const
    {
        return 5;
    }

    template <class Serializer>
    spp::Error serialize(Serializer& serializer) const
    {
        return serializer
        (
            SPP_OPT(floatData)
        );
    }

    template <class Serializer>
    spp::Error deserialize(Serializer& serializer, spp::Version archiveVersion)
    {
        assert(archiveVersion == actualDataVersion());
        return serialize(serializer);
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

            constexpr spp::Version actualDataVersion() const
            {
                return 1;
            }

            template <class Serializer>
            spp::Error serialize(Serializer& serializer) const
            {
                return serializer
                (
                    SPP_OPT(b),
                    SPP_MDR(e1),
                    SPP_MDR(e2)
                );
            }

            template <class Serializer>
            spp::Error deserialize(Serializer& serializer, spp::Version archiveVersion)
            {
                assert(archiveVersion == actualDataVersion());
                return serialize(serializer);
            }
        };

        C c;

        constexpr spp::Version actualDataVersion() const
        {
            return 2;
        }

        template <class Serializer>
        spp::Error serialize(Serializer& serializer) const
        {
            return serializer
            (
                SPP_MDR(c)
            );
        }

        template <class Serializer>
        spp::Error deserialize(Serializer& serializer, spp::Version archiveVersion)
        {
            assert(archiveVersion == actualDataVersion());
            return serialize(serializer);
        }
    };

    B b;

    struct Point
    {
        int x;
        int y;

        constexpr spp::Version actualDataVersion() const
        {
            return 1;
        }

        template <class Serializer>
        spp::Error serialize(Serializer& serializer) const
        {
            return serializer
            (
                SPP_MDR(x),
                SPP_OPT(y)
            );
        }

        template <class Serializer>
        spp::Error deserialize(Serializer& serializer, spp::Version archiveVersion)
        {
            assert(archiveVersion == actualDataVersion());
            return serialize(serializer);
        }
    };

    std::vector<Point> points = { { 1, 2}, { 3, 4 } };

    std::map<short, std::vector<char>> dict = { { 1, { 'a', 'b' } }, { -5, {} } };

    constexpr spp::Version actualDataVersion() const
    {
        return 1;
    }

    template <class Serializer>
    spp::Error serialize(Serializer& serializer) const
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

    template <class Serializer>
    spp::Error deserialize(Serializer& serializer, spp::Version archiveVersion)
    {
        assert(archiveVersion == actualDataVersion());
        return serialize(serializer);
    }
};
