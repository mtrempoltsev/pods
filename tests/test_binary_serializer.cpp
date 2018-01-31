#include <gtest/gtest.h>

#include <pods/binary_serializer.h>
#include <pods/memory_storage.h>

#include "data.h"

TEST(binarySerializer, common)
{
    const TestData expected;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    TestData actual;

    actual.x = 0;
    actual.y = 0;
    actual.z = std::string();
    actual.a.floatData = { 0, 0, 0, 0, 0 };
    actual.b.c.b = true;
    actual.b.c.e1 = AA;
    actual.b.c.e2 = Enum2::Y;
    actual.points.clear();
    actual.dict.clear();
    actual.strings.clear();
    actual.arr = { { 0, 0 } };
    actual.map.clear();

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
    EXPECT_EQ(expected.y, actual.y);
    EXPECT_EQ(expected.z, actual.z);
    EXPECT_EQ(expected.a.floatData, actual.a.floatData);
    EXPECT_EQ(expected.b.c.b, actual.b.c.b);
    EXPECT_EQ(expected.b.c.e1, actual.b.c.e1);
    EXPECT_EQ(expected.b.c.e2, actual.b.c.e2);
    EXPECT_EQ(expected.points, actual.points);
    EXPECT_EQ(expected.dict, actual.dict);
    EXPECT_EQ(expected.strings, actual.strings);
    EXPECT_EQ(expected.arr, actual.arr);
    EXPECT_EQ(expected.map, actual.map);
}
