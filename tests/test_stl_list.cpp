#include <gtest/gtest.h>

#include <list>

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

struct List
{
    static constexpr int aSize = 15;
    static constexpr int bSize = 16;
    static constexpr int cSize = 65536;
    static constexpr int dSize = 2;

    std::list<std::string> a;
    std::list<SimpleSerializable> b;
    std::list<int32_t> c;
    using IntList = std::list<int64_t>;
    std::list<IntList> d;
    std::list<int32_t> e;

    void fill()
    {
        for (int i = 0; i < aSize; ++i)
        {
            a.push_back(std::to_string(i));
        }

        for (int i = 0; i < bSize; ++i)
        {
            b.push_back(SimpleSerializable{ i });
        }

        for (int i = 0; i < cSize; ++i)
        {
            c.push_back(i);
        }

        d = { { { 1, 2 }, { 3, 4 } } };
    }

    PODS_SERIALIZABLE(PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d), PODS_MDR(e))
};

template <class Serializer, class Deserializer>
void testStlList()
{
    List expected{};
    expected.fill();

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    List actual{};
    actual.e.push_back(10);

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
    EXPECT_EQ(expected.e, actual.e);
}

TEST(msgpack, testStlList)
{
    testStlList<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(json, testStlList)
{
    testStlList<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}
