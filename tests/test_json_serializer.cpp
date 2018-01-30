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

    const auto json = std::string(out.data(), out.size());
}
