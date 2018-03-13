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
