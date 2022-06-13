#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
#include <pods/pods.h>

struct Float
{
    float a = 3.4e38f;
    double b = 1.7e308;

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

TEST(json, testFloat)
{
    testFloat<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testFloat)
{
    testFloat<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
