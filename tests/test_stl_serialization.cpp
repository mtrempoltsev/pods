#include <gtest/gtest.h>

#include <pods/binary.h>
#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>

#include "stl_data.h"

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

    EXPECT_EQ(expected.x, actual.x);
    EXPECT_EQ(expected.y, actual.y);
    EXPECT_EQ(expected.z, actual.z);
}

TEST(stlSerialization, binary)
{
    using BinarySerializer = pods::BinarySerializer<pods::ResizableOutputBuffer>;
    using BinaryDeserializer = pods::BinaryDeserializer<pods::InputBuffer>;

    test<StlArray, BinarySerializer, BinaryDeserializer>();
    test<StlVector, BinarySerializer, BinaryDeserializer>();
    test<StlDeque, BinarySerializer, BinaryDeserializer>();
    test<StlList, BinarySerializer, BinaryDeserializer>();
    test<StlForwardList, BinarySerializer, BinaryDeserializer>();
}

TEST(stlSerialization, json)
{
    using JsonSerializer = pods::JsonSerializer<pods::ResizableOutputBuffer>;
    using JsonDeserializer = pods::JsonDeserializer<pods::InputBuffer>;

    test<StlArray, JsonSerializer, JsonDeserializer>();
    test<StlVector, JsonSerializer, JsonDeserializer>();
    test<StlDeque, JsonSerializer, JsonDeserializer>();
    test<StlList, JsonSerializer, JsonDeserializer>();
    test<StlForwardList, JsonSerializer, JsonDeserializer>();
}

TEST(stlSerialization, msgpack)
{
    using MsgPackSerializer = pods::MsgPackSerializer<pods::ResizableOutputBuffer>;
    using MsgPackDeserializer = pods::MsgPackDeserializer<pods::InputBuffer>;

    test<StlArray, MsgPackSerializer, MsgPackDeserializer>();
    test<StlVector, MsgPackSerializer, MsgPackDeserializer>();
    test<StlDeque, MsgPackSerializer, MsgPackDeserializer>();
    test<StlList, MsgPackSerializer, MsgPackDeserializer>();
    test<StlForwardList, MsgPackSerializer, MsgPackDeserializer>();
}
