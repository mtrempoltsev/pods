#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
#include <pods/pods.h>

struct String
{
    std::string a;
    std::string b = "some text";
    std::string c = std::string(std::numeric_limits<uint8_t>::max(), 'c');
    std::string d = std::string(std::numeric_limits<uint8_t>::max() + 1, 'd');
    std::string e = std::string(std::numeric_limits<uint16_t>::max() + 1, 'e');

    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d), PODS_MDR(e))
};

template <class Serializer, class Deserializer>
void testString()
{
    const String expected;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    String actual;
    actual.a = "fail";
    actual.b.clear();
    actual.c.clear();
    actual.d.clear();
    actual.e.clear();

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
    EXPECT_EQ(expected.e, actual.e);
}

TEST(json, testString)
{
    testString<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testString)
{
    testString<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
