#include <gtest/gtest.h>

#include <array>

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

    std::array<std::string, aSize> a;
    std::array<SimpleSerializable, bSize> b;
    std::array<int32_t, cSize> c;
    using IntArray = std::array<int, dSize>;
    std::array<IntArray, dSize> d;

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

        d = { { { 1, 2 }, { 3, 4 } } };
    }

    PODS_SERIALIZABLE(PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d))
};

template <class Serializer, class Deserializer>
void testStlArray()
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

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
}

TEST(msgpack, testStlArray)
{
    testStlArray<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(json, testStlArray)
{
    testStlArray<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}
