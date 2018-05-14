#pragma once

#include <cstdint>
#include <string>
#include <map>

#include <pods/pods.h>

struct Bool
{
    bool a = true;
    bool b = false;

    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b))
};

template <class Serializer, class Deserializer>
void testBool()
{
    const Bool expected;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Bool actual;
    actual.a = false;
    actual.b = true;

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
}

struct Int
{
    int8_t a = std::numeric_limits<int8_t>::min();
    int8_t b = std::numeric_limits<int8_t>::max();
    int8_t c = -32;
    int8_t d = -33;

    int16_t e = std::numeric_limits<int16_t>::min();
    int16_t f = std::numeric_limits<int16_t>::max();

    int32_t g = std::numeric_limits<int32_t>::min();
    int32_t h = std::numeric_limits<int32_t>::max();

    int64_t i = std::numeric_limits<int64_t>::min();
    int64_t k = std::numeric_limits<int64_t>::max();

    int64_t l = a;
    int64_t m = b;
    int64_t n = e;
    int64_t o = f;
    int64_t p = g;
    int64_t q = h;

    PODS_SERIALIZABLE(1,
        PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d), PODS_MDR(e), PODS_MDR(f), PODS_MDR(g),
        PODS_MDR(h), PODS_MDR(i), PODS_MDR(k), PODS_MDR(l), PODS_MDR(m), PODS_MDR(n), PODS_MDR(o),
        PODS_MDR(p), PODS_MDR(q))
};

template <class Serializer, class Deserializer>
void testInt()
{
    const Int expected;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Int actual;
    memset(&actual, 0, sizeof(Int));

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
    EXPECT_EQ(expected.e, actual.e);
    EXPECT_EQ(expected.f, actual.f);
    EXPECT_EQ(expected.g, actual.g);
    EXPECT_EQ(expected.h, actual.h);
    EXPECT_EQ(expected.i, actual.i);
    EXPECT_EQ(expected.k, actual.k);
    EXPECT_EQ(expected.l, actual.l);
    EXPECT_EQ(expected.m, actual.m);
    EXPECT_EQ(expected.n, actual.n);
    EXPECT_EQ(expected.o, actual.o);
    EXPECT_EQ(expected.p, actual.p);
    EXPECT_EQ(expected.q, actual.q);
}

struct Unsigned
{
    uint8_t a = 127;
    uint8_t b = std::numeric_limits<uint8_t>::max();
    uint16_t c = std::numeric_limits<uint16_t>::max();
    uint32_t d = std::numeric_limits<uint32_t>::max();
    uint64_t e = std::numeric_limits<uint64_t>::max();

    uint64_t f = a;
    uint64_t g = b;
    uint64_t h = c;
    uint64_t i = d;

    PODS_SERIALIZABLE(1,
        PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d), PODS_MDR(e), PODS_MDR(f), PODS_MDR(g),
        PODS_MDR(h), PODS_MDR(i))
};

template <class Serializer, class Deserializer>
void testUnsigned()
{
    const Unsigned expected;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Unsigned actual;
    memset(&actual, 0, sizeof(Unsigned));

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
    EXPECT_EQ(expected.e, actual.e);
    EXPECT_EQ(expected.f, actual.f);
    EXPECT_EQ(expected.g, actual.g);
    EXPECT_EQ(expected.h, actual.h);
    EXPECT_EQ(expected.i, actual.i);
}

struct Float
{
    float a = std::numeric_limits<float>::max();
    double b = std::numeric_limits<double>::max();

    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b))
};

template <class Serializer, class Deserializer>
void testFloat()
{
    const Float expected;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Float actual;
    memset(&actual, 0, sizeof(Float));

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_FLOAT_EQ(expected.a, actual.a);
    EXPECT_DOUBLE_EQ(expected.b, actual.b);
}

struct String
{
    std::string a;
    std::string b = "some text";
    std::string c = std::string(std::numeric_limits<uint8_t>::max(), 'c');
    std::string d = std::string(std::numeric_limits<uint8_t>::max() + 1, 'd');
    std::string e = std::string(std::numeric_limits<uint16_t>::max() + 1, 'e');

    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d), PODS_MDR(e))
};

template <class Serializer, class Deserializer>
void testString()
{
    const String expected;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    String actual;
    actual.a = "fail";
    actual.b.clear();
    actual.c.clear();
    actual.d.clear();
    actual.e.clear();

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
    EXPECT_EQ(expected.e, actual.e);
}

struct Binary
{
    static constexpr int size = 3;

    std::vector<uint32_t> a; // zero size
    std::vector<uint32_t> b = { 0xfedcba98, 0x76543210, 0x648823fe };
    std::unique_ptr<int16_t[]> c;
    int16_t* d;
    std::array<uint32_t, 3> e = { { 0xfedcba98, 0x76543210, 0x648823fe } };
    std::array<uint8_t, 1> f = { { 100 } };
    uint64_t g[4] = { 1, 2, 3, 4 };
    std::vector<uint8_t> h;
    std::vector<uint8_t> i;

    static constexpr int hSize = 65535;
    static constexpr int iSize = 65536;

    void fill()
    {
        for (size_t j = 0; j < size; ++j)
        {
            c.get()[j] = static_cast<int16_t>(j + 1);
            d[j] = static_cast<int16_t>(j + 10);
        }

        uint8_t n = 0;

        for (int j = 0; j < hSize; ++j)
        {
            h.push_back(n++);
        }

        for (int j = 0; j < iSize; ++j)
        {
            i.push_back(n++);
        }
    }

    PODS_SERIALIZABLE(1, PODS_MDR_BIN(a), PODS_MDR_BIN(b), PODS_MDR_BIN_2(c, size),
        PODS_MDR_BIN_2(d, size), PODS_MDR_BIN(e), PODS_MDR_BIN(f), PODS_MDR_BIN(g),
        PODS_MDR_BIN(h), PODS_MDR_BIN(i))
};

template <class Serializer, class Deserializer>
void testBinary()
{
    int16_t buf1[Binary::size];
    int16_t buf2[Binary::size];

    Binary expected;
    expected.c = std::make_unique<int16_t[]>(expected.size);
    expected.d = buf1;

    expected.fill();

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Binary actual{};
    actual.a.push_back(100);
    actual.b.clear();
    actual.c = std::make_unique<int16_t[]>(expected.size);
    std::fill_n(actual.c.get(), expected.size, 0);
    actual.d = buf2;
    std::fill_n(actual.d, expected.size, 0);
    std::fill_n(actual.e.data(), actual.e.size(), 0);
    std::fill_n(actual.f.data(), actual.f.size(), 0);
    std::fill_n(actual.g, sizeof(actual.g) / sizeof(actual.g[0]), 0);

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);

    for (size_t i = 0; i < expected.size; ++i)
    {
        EXPECT_EQ(expected.c.get()[i], actual.c.get()[i]);
        EXPECT_EQ(expected.d[i], actual.d[i]);
    }

    EXPECT_EQ(expected.e, actual.e);
    EXPECT_EQ(expected.f, actual.f);

    for (size_t i = 0; i < sizeof(actual.g) / sizeof(actual.g[0]); ++i)
    {
        EXPECT_EQ(expected.g[i], actual.g[i]);
    }

    EXPECT_EQ(expected.h[0], actual.h[0]);
    EXPECT_EQ(expected.h[Binary::hSize - 1], actual.h[Binary::hSize - 1]);

    EXPECT_EQ(expected.i[0], actual.i[0]);
    EXPECT_EQ(expected.i[Binary::hSize - 1], actual.i[Binary::hSize - 1]);
    EXPECT_EQ(expected.i[Binary::hSize], actual.i[Binary::hSize]);
    EXPECT_EQ(expected.i[Binary::iSize - 1], actual.i[Binary::iSize - 1]);
}

struct SimpleSerializable
{
    int64_t a;

    PODS_SERIALIZABLE(1, PODS_MDR(a))

    bool operator==(const SimpleSerializable& right) const
    {
        return a == right.a;
    }
};

struct Array
{
    static constexpr int aSize = 15;
    static constexpr int bSize = 16;
    static constexpr int cSize = 65536;

    std::string a[aSize];
    SimpleSerializable b[bSize];
    int32_t c[cSize];

    void fill()
    {
        for (int i = 0; i < aSize; ++i)
        {
            a[i] = std::to_string(i);
        }

        for (int i = 0; i < bSize; ++i)
        {
            b[i] = SimpleSerializable { i };
        }

        for (int i = 0; i < cSize; ++i)
        {
            c[i] = i;
        }
    }

    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b), PODS_MDR(c))
};

template <class Serializer, class Deserializer>
void testArray()
{
    Array expected{};
    expected.fill();

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Array actual{};

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a[0], actual.a[0]);
    EXPECT_EQ(expected.a[Array::aSize - 1], actual.a[Array::aSize - 1]);

    EXPECT_EQ(expected.b[0], actual.b[0]);
    EXPECT_EQ(expected.b[Array::aSize - 1], actual.b[Array::aSize - 1]);
    EXPECT_EQ(expected.b[Array::aSize], actual.b[Array::aSize]);
    EXPECT_EQ(expected.b[Array::bSize - 1], actual.b[Array::bSize - 1]);

    EXPECT_EQ(expected.c[0], actual.c[0]);
    EXPECT_EQ(expected.c[Array::aSize - 1], actual.c[Array::aSize - 1]);
    EXPECT_EQ(expected.c[Array::aSize], actual.c[Array::aSize]);
    EXPECT_EQ(expected.c[Array::bSize - 1], actual.c[Array::bSize - 1]);
    EXPECT_EQ(expected.c[Array::bSize], actual.c[Array::bSize]);
    EXPECT_EQ(expected.c[Array::cSize - 1], actual.c[Array::cSize - 1]);
}

struct Map
{
    static constexpr int aSize = 15;
    static constexpr int bSize = 16;
    static constexpr int cSize = 65536;

    std::map<int, int> a;
    std::map<int, int> b;
    std::map<int, int> c;

    void fill()
    {
        for (int i = 0; i < aSize; ++i)
        {
            a[i] = i;
        }

        for (int i = 0; i < bSize; ++i)
        {
            b[i] = i * 2;
        }

        for (int i = 0; i < cSize; ++i)
        {
            c[i] = i * 3;
        }
    }

    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b), PODS_MDR(c))
};

template <class Serializer, class Deserializer>
void testMap()
{
    Map expected{};
    expected.fill();

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Map actual{};

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a[0], actual.a[0]);
    EXPECT_EQ(expected.a[Map::aSize - 1], actual.a[Map::aSize - 1]);

    EXPECT_EQ(expected.b[0], actual.b[0]);
    EXPECT_EQ(expected.b[Map::aSize - 1], actual.b[Map::aSize - 1]);
    EXPECT_EQ(expected.b[Map::aSize - 0], actual.b[Map::aSize - 0]);
    EXPECT_EQ(expected.b[Map::bSize - 1], actual.b[Map::bSize - 1]);

    EXPECT_EQ(expected.c[0], actual.c[0]);
    EXPECT_EQ(expected.c[Map::aSize - 1], actual.c[Map::aSize - 1]);
    EXPECT_EQ(expected.c[Map::aSize - 0], actual.c[Map::aSize - 0]);
    EXPECT_EQ(expected.c[Map::bSize - 1], actual.c[Map::bSize - 1]);
    EXPECT_EQ(expected.c[Map::bSize - 0], actual.c[Map::bSize - 0]);
    EXPECT_EQ(expected.c[Map::cSize - 1], actual.c[Map::cSize - 1]);
}
