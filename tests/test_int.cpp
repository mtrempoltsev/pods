#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
#include <pods/pods.h>

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

TEST(json, testInt)
{
    testInt<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testInt)
{
    testInt<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
