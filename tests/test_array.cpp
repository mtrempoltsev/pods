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
};

struct Array
{
    static constexpr int aSize = 15;
    static constexpr int bSize = 16;
    static constexpr int cSize = 65536;
    static constexpr int dSize = 2;

    std::string a[aSize];
    SimpleSerializable b[bSize];
    int32_t c[cSize];
    using IntArray = int[dSize];
    IntArray d[dSize];

    void fill()
    {
        for (int i = 0; i < aSize; ++i)
        {
            a[i] = std::to_string(i);
        }

        for (int i = 0; i < bSize; ++i)
        {
            b[i] = SimpleSerializable{ i };
        }

        for (int i = 0; i < cSize; ++i)
        {
            c[i] = i;
        }

        for (int i = 0; i < dSize; ++i)
        {
            for (int j = 0; j < dSize; ++j)
            {
                d[i][j] = i + j;
            }
        }
    };

    PODS_SERIALIZABLE(PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d))
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

    EXPECT_EQ(expected.d[0][0], actual.d[0][0]);
    EXPECT_EQ(expected.d[1][0], actual.d[1][0]);
    EXPECT_EQ(expected.d[0][1], actual.d[0][1]);
    EXPECT_EQ(expected.d[1][1], actual.d[1][1]);
}

TEST(msgpack, testArray)
{
    testArray<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(json, testArray)
{
    testArray<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}
