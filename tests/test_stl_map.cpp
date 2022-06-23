#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
#include <pods/pods.h>

struct SimpleSerializable
{
    int64_t a;

    PODS_SERIALIZABLE(PODS_MDR(a))

    bool operator==(const SimpleSerializable& right) const
    {
        return a == right.a;
    }

    bool operator<(const SimpleSerializable right) const
    {
        return a < right.a;
    }
};

struct Map
{
    static constexpr int aSize = 15;
    static constexpr int bSize = 16;
    static constexpr int cSize = 65536;

    std::map<int64_t, std::array<int32_t, 1>> a;
    std::map<int64_t, SimpleSerializable> b;
    std::map<int64_t, int64_t> c;
    std::map<SimpleSerializable, int64_t> d;
    std::map<SimpleSerializable, SimpleSerializable> e;
    std::map<std::map<int32_t, int32_t>, std::map<int32_t, int32_t>> f;
    std::map<int32_t, int32_t> g;


    void fill()
    {
        for (int i = 0; i < aSize; ++i)
        {
            a[i] = { i };
        }

        for (int i = 0; i < bSize; ++i)
        {
            b[i] = { i * 2 };
        }

        for (int i = 0; i < cSize; ++i)
        {
            c[i] = i * 3;
        }

        d = { { { 1 }, 2 } };
        e = { { { 1 }, { 2 } } };
        f = { { { { 1, 2 } }, { { 3, 4 } } } };
    }

    PODS_SERIALIZABLE(PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d), PODS_MDR(e), PODS_MDR(f), PODS_MDR(g))
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
    actual.g[1] = 1;

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
}

struct StringMap
{
    static constexpr int aSize = 15;
    static constexpr int bSize = 16;
    static constexpr int cSize = 65536;

    std::map<std::string, std::array<int32_t, 1>> a;
    std::map<std::string, SimpleSerializable> b;
    std::map<std::string, int64_t> c;
    std::map<std::string, int32_t> d;

    void fill()
    {
        for (int i = 0; i < aSize; ++i)
        {
            a[std::to_string(i)] = {i};
        }

        for (int i = 0; i < bSize; ++i)
        {
            b[std::to_string(i)] = { i * 2 };
        }

        for (int i = 0; i < cSize; ++i)
        {
            c[std::to_string(i)] = i * 3;
        }
    }

    PODS_SERIALIZABLE(PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d))
};

template <class Serializer, class Deserializer>
void testStringMap()
{
    StringMap expected{};
    expected.fill();

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    StringMap actual{};
    actual.d["a"] = 1;

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
}

TEST(json, testMap)
{
    testStringMap<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testMap)
{
    testMap<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
    testStringMap<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
