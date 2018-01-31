#include <gtest/gtest.h>

#include <pods/json_serializer.h>
#include <pods/memory_storage.h>

#include "data.h"

TEST(jsonSerializer, common)
{
    const TestData expected;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::JsonSerializer<pods::ResizeableWriteOnlyMemoryStorage> serializer(out);
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

    const std::string json = std::string(out.data(), out.size());

    const std::string expectedJson = "{\"version\":1,\"x\":100,\"y\":200.5,\"z\":\"hello!\",\"a\":{\"version\":5,\"floatData\":[1.100000023841858,2.200000047683716,3.299999952316284,4.400000095367432,5.5]},\"b\":{\"version\":2,\"c\":{\"version\":1,\"b\":false,\"e1\":1,\"e2\":0}},\"points\":{\"version\":1,\"data\":[{\"x\":1,\"y\":2},{\"x\":3,\"y\":4}]},\"dict\":[{\"key\":-5,\"value\":[]},{\"key\":1,\"value\":[97,98]}],\"strings\":[\"hello\",\"мир\"],\"arr\":{\"version\":1,\"data\":[{\"x\":5,\"y\":6}]},\"map\":[{\"key\":{\"version\":1,\"x\":7,\"y\":8},\"value\":{\"version\":1,\"x\":9,\"y\":10}}]}";

    EXPECT_EQ(expectedJson, json);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::JsonDeserializer<pods::ReadOnlyMemoryStorage> deserializer(in);
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

TEST(jsonSerializer, error)
{
    const TestData expected;
    pods::FixedSizeWriteOnlyMemoryStorage out(128);
    pods::PrettyJsonSerializer<pods::FixedSizeWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::WriteError);
}
