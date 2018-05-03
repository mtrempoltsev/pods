#include <gtest/gtest.h>

#include <pods/buffers.h>

#include "storage_data.h"

TEST(resizeableBuffer, testSigned)
{
    pods::ResizableOutputBuffer out;
    testSignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testSignedRead(in);
}

TEST(resizeableBuffer, testUnsigned)
{
    pods::ResizableOutputBuffer out;
    testUnsignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST(resizeableBuffer, testFloat)
{
    pods::ResizableOutputBuffer out;
    testFloatWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testFloatRead(in);
}

TEST(resizeableBuffer, testCharBool)
{
    pods::ResizableOutputBuffer out;
    testBoolWrite(out);
    testCharWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testBoolRead(in);
    testCharRead(in);
}

TEST(resizeableBuffer, testRawData)
{
    pods::ResizableOutputBuffer out;
    testRawDataWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testRawDataRead(in);
}

TEST(resizeableBuffer, testError)
{
    uint16_t small = 0;
    uint64_t big = 0;

    {
        pods::ResizableOutputBuffer out(4, 4);
        EXPECT_EQ(out.put(big), pods::Error::NotEnoughMemory);
    }

    {
        pods::ResizableOutputBuffer out;
        EXPECT_EQ(out.put(small), pods::Error::NoError);

        pods::InputBuffer in(out.data(), out.size());
        EXPECT_EQ(in.get(big), pods::Error::UnexpectedEnd);
    }
}

TEST(resizeableBuffer, resize)
{
    const uint64_t e1 = 1;
    const uint64_t e2 = 2;
    const uint64_t e3 = 3;
    const uint64_t e4 = 4;
    const uint64_t e5 = 5;

    pods::ResizableOutputBuffer out(10);

    EXPECT_EQ(out.size(), 0);

    EXPECT_EQ(out.put(e1), pods::Error::NoError);
    EXPECT_EQ(out.size(), 8);
    EXPECT_EQ(out.capacity(), 10);

    EXPECT_EQ(out.put(e2), pods::Error::NoError);
    EXPECT_EQ(out.size(), 16);
    EXPECT_EQ(out.capacity(), 32);

    EXPECT_EQ(out.put(e3), pods::Error::NoError);
    EXPECT_EQ(out.size(), 24);
    EXPECT_EQ(out.capacity(), 32);

    EXPECT_EQ(out.put(e4), pods::Error::NoError);
    EXPECT_EQ(out.size(), 32);
    EXPECT_EQ(out.capacity(), 32);

    EXPECT_EQ(out.put(e5), pods::Error::NoError);
    EXPECT_EQ(out.size(), 40);
    EXPECT_EQ(out.capacity(), 80);

    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t a4 = 0;
    uint64_t a5 = 0;

    pods::InputBuffer in(out.data(), out.size());

    EXPECT_EQ(in.get(a1), pods::Error::NoError);
    EXPECT_EQ(e1, a1);
    EXPECT_EQ(in.get(a2), pods::Error::NoError);
    EXPECT_EQ(e2, a2);
    EXPECT_EQ(in.get(a3), pods::Error::NoError);
    EXPECT_EQ(e3, a3);
    EXPECT_EQ(in.get(a4), pods::Error::NoError);
    EXPECT_EQ(e4, a4);
    EXPECT_EQ(in.get(a5), pods::Error::NoError);
    EXPECT_EQ(e5, a5);
    EXPECT_EQ(in.get(a1), pods::Error::UnexpectedEnd);
}
