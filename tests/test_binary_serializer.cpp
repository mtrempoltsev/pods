#include <gtest/gtest.h>

#include <serialization++/binary_serializer.h>
#include <serialization++/memory_storage.h>

#include "data.h"

TEST(binarySerializer, common)
{
    TestData expected;

    spp::ResizeableWriteOnlyMemoryStorage out;
    spp::BinarySerializer<spp::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), spp::Error::NoError);

    TestData actual;

    actual.x = 0;
    actual.y = 0;
    actual.z = std::string();
    actual.a.floatData = { 0, 0, 0, 0, 0 };
    actual.b.c.b = true;
    actual.b.c.e1 = AA;
    actual.b.c.e2 = Enum2::Y;
    actual.points = std::vector<TestData::Point>();
    actual.dict = std::map<short, std::vector<char>>();

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    spp::BinaryDeserializer<spp::ReadOnlyMemoryStorage> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), spp::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
    EXPECT_EQ(expected.y, actual.y);
    EXPECT_EQ(expected.z, actual.z);
    EXPECT_EQ(expected.a.floatData, actual.a.floatData);
    EXPECT_EQ(expected.b.c.b, actual.b.c.b);
    EXPECT_EQ(expected.b.c.e1, actual.b.c.e1);
    EXPECT_EQ(expected.b.c.e2, actual.b.c.e2);
    EXPECT_EQ(expected.points, actual.points);
    EXPECT_EQ(expected.dict, actual.dict);
}
