#include <gtest/gtest.h>

#include <pods/binary.h>
#include <pods/buffers.h>
#include <pods/json.h>

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

    test<Array, BinarySerializer, BinaryDeserializer>();
    test<Vector, BinarySerializer, BinaryDeserializer>();
    test<Deque, BinarySerializer, BinaryDeserializer>();
    test<List, BinarySerializer, BinaryDeserializer>();
    test<ForwardList, BinarySerializer, BinaryDeserializer>();
}

TEST(stlSerialization, json)
{
    using JsonSerializer = pods::JsonSerializer<pods::ResizableOutputBuffer>;
    using JsonDeserializer = pods::JsonDeserializer<pods::InputBuffer>;

    test<Array, JsonSerializer, JsonDeserializer>();
    test<Vector, JsonSerializer, JsonDeserializer>();
}
