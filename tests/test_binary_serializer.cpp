#include <gtest/gtest.h>

#include <serialization++/binary_serializer.h>
#include <serialization++/memory_storage.h>

#include "data.h"

TEST(binarySerializer, common)
{
    const TestData expected;

    spp::ResizeableWriteOnlyMemoryStorage out;
    spp::BinarySerializer<spp::ResizeableWriteOnlyMemoryStorage> serializer(out);
    EXPECT_EQ(serializer.save(expected), spp::Error::NoError);

    TestData actual;

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    spp::BinaryDeserializer<spp::ReadOnlyMemoryStorage> deserializer(in);
    //deserializer.load(actual);
}
