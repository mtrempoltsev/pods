#include <gtest/gtest.h>

#include <forward_list>

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

struct ForwardList
{
    static constexpr int aSize = 15;
    static constexpr int bSize = 16;
    static constexpr int cSize = 65536;
    static constexpr int dSize = 2;

    std::forward_list<std::string> a;
    std::forward_list<SimpleSerializable> b;
    std::forward_list<int32_t> c;
    using IntForwardList = std::forward_list<int64_t>;
    std::forward_list<IntForwardList> d;
    std::forward_list<int32_t> e;

    void fill()
    {
        for (int i = 0; i < aSize; ++i)
        {
            a.push_front(std::to_string(i));
        }

        for (int i = 0; i < bSize; ++i)
        {
            b.push_front(SimpleSerializable{ i });
        }

        for (int i = 0; i < cSize; ++i)
        {
            c.push_front(i);
        }

        d = { { { 1, 2 }, { 3, 4 } } };
    }

    PODS_SERIALIZABLE(PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d), PODS_MDR(e))
};

template <class Serializer, class Deserializer>
void testStlForwardList()
{
    ForwardList expected{};
    expected.fill();

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    ForwardList actual{};
    actual.e.push_front(10);

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
    EXPECT_EQ(expected.e, actual.e);
}

TEST(msgpack, testStlForwardList)
{
    testStlForwardList<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(json, testStlForwardList)
{
    testStlForwardList<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}
