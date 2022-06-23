#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
#include <pods/pods.h>

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

    PODS_SERIALIZABLE(PODS_MDR(a), PODS_MDR(b), PODS_MDR(c))
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

TEST(json, testMap)
{
    testMap<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testMap)
{
    testMap<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
