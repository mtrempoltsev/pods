#include <gtest/gtest.h>

#include <pods/binary.h>
#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>

#include "stl_map_data.h"

template <class Object, class Serializer, class Deserializer>
void test()
{
    const Object expected;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);

    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Object actual;
    clear(actual);

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);
    EXPECT_EQ(expected.c, actual.c);
    EXPECT_EQ(expected.d, actual.d);
    EXPECT_EQ(expected.e, actual.e);
}

TEST(stlMapSerialization, binary)
{
    using BinarySerializer = pods::BinarySerializer<pods::ResizableOutputBuffer>;
    using BinaryDeserializer = pods::BinaryDeserializer<pods::InputBuffer>;

    test<StlMap, BinarySerializer, BinaryDeserializer>();
}

TEST(stlMapSerialization, json)
{
    using JsonSerializer = pods::JsonSerializer<pods::ResizableOutputBuffer>;
    using JsonDeserializer = pods::JsonDeserializer<pods::InputBuffer>;

    test<StlMap, JsonSerializer, JsonDeserializer>();
}

TEST(stlMapSerialization, msgpack)
{
    using MsgPackSerializer = pods::MsgPackSerializer<pods::ResizableOutputBuffer>;
    using MsgPackDeserializer = pods::MsgPackDeserializer<pods::InputBuffer>;

    test<StlMap, MsgPackSerializer, MsgPackDeserializer>();
}
