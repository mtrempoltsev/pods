#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>

#include "versioning_data.h"

TEST(versioning, json_err_1)
{
    const A2 original;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<decltype(out)> serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A1 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<decltype(in)> deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::ArchiveVersionMismatch);
}

TEST(versioning, json_err_2)
{
    const A3 original;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<decltype(out)> serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A2 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<decltype(in)> deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::ArchiveVersionMismatch);
}

TEST(versioning, json_ok_1)
{
    const A1 original;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<decltype(out)> serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A2 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<decltype(in)> deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(actual.x, 1);
    EXPECT_EQ(actual.y, 5);
    EXPECT_EQ(actual.b.a, 10);
}

TEST(versioning, json_ok_2)
{
    const A2 original;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<decltype(out)> serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A3 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<decltype(in)> deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(actual.x, 3);
    EXPECT_EQ(actual.y, 5);
    EXPECT_EQ(actual.b.a, 10);
    EXPECT_EQ(actual.b.b, 12);
}
