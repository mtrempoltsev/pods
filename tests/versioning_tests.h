#pragma once

#include "versioning_data.h"

template <class Serializer, class Deserializer>
void testErr1()
{
    const A2 original;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A1 actual;

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::ArchiveVersionMismatch);
}

template <class Serializer, class Deserializer>
void testErr2()
{
    const A3 original;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A2 actual;

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::ArchiveVersionMismatch);
}

template <class Serializer, class Deserializer>
void testOk1()
{
    const A1 original;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A2 actual;

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(actual.x, 1);
    EXPECT_EQ(actual.y, 5);
    EXPECT_EQ(actual.b.a, 10);
}

template <class Serializer, class Deserializer>
void testOk2()
{
    const A2 original;

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A3 actual;

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(actual.x, 3);
    EXPECT_EQ(actual.y, 5);
    EXPECT_EQ(actual.b.a, 10);
    EXPECT_EQ(actual.b.b, 12);
}
