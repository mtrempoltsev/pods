#pragma once

#include <array>
#include <map>
#include <memory>
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

    std::array<float, 5> floatData = { { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f } };

    static constexpr pods::Version version()
    {
        return 5;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version /*archiveVersion*/)
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
            pods::Error serialize(Serializer& serializer, pods::Version /*archiveVersion*/)
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
        pods::Error serialize(Serializer& serializer, pods::Version /*archiveVersion*/)
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

        bool operator<(const Point other) const
        {
            return x < other.x && y < other.y;
        }

        bool operator==(const Point other) const
        {
            return x == other.x && y == other.y;
        }

        static constexpr pods::Version version()
        {
            return 1;
        }

        template <class Serializer>
        pods::Error serialize(Serializer& serializer, pods::Version /*archiveVersion*/)
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

    std::vector<std::string> strings = { "hello", "мир" };

    std::array<Point, 1> arr = { { { 5, 6 } } };

    std::map<Point, Point> map = { { { 7, 8 }, { 9, 10 } } };

    static constexpr pods::Version version()
    {
        return 1;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version /*archiveVersion*/)
    {
        return serializer
        (
            PODS_MDR(x),
            PODS_OPT(y),
            PODS_MDR(z),
            PODS_OPT(a),
            PODS_MDR(b),
            PODS_OPT(points),
            PODS_MDR(dict),
            PODS_MDR(strings),
            PODS_OPT(arr),
            PODS_OPT(map)
        );
    }
};

struct BinData1
{
    std::vector<uint32_t> x = { 0xfedcba98, 0x76543210, 0x648823fe };
    PODS_SERIALIZABLE(1, PODS_MDR_BIN(x))
};

struct BinData2
{
    static constexpr int size = 3;
    std::unique_ptr<int16_t[]> x;
    int16_t* y;
    PODS_SERIALIZABLE(1,
        PODS_MDR_BIN_2(x, size),
        PODS_MDR_BIN_2(y, size))
};

struct Array1
{
    std::array<uint32_t, 3> x = { { 0xfedcba98, 0x76543210, 0x648823fe } };
    PODS_SERIALIZABLE(1, PODS_MDR_BIN(x))
};

struct Array2
{
    std::array<uint32_t, 1> x = { { 100 } };
    PODS_SERIALIZABLE(1, PODS_MDR_BIN(x))
};

struct CArray
{
    bool ok = true;
    uint32_t x[4] = { 1, 2, 3, 4 };
    PODS_SERIALIZABLE(1, PODS_MDR(x), PODS_MDR(ok))
};

struct CArrayBin
{
    bool ok = true;
    uint32_t x[4] = { 1, 2, 3, 4 };
    PODS_SERIALIZABLE(1, PODS_MDR_BIN(x), PODS_MDR(ok))
};

struct InvalidSize
{
    char data[1] = { '\0' };
    PODS_SERIALIZABLE(1, PODS_MDR_BIN_2(data, static_cast<size_t>(std::numeric_limits<uint64_t>::max())))
};
