#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
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

TEST(json, testBool)
{
    testBool<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testBool)
{
    testBool<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
