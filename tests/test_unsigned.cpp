#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
#include <pods/pods.h>

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

TEST(json, testUnsigned)
{
    testUnsigned<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testUnsigned)
{
    testUnsigned<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
