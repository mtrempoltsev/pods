#include <gtest/gtest.h>

#include <pods/buffers.h>

#include "storage_data.h"

TEST(resizeableMemoryStorage, testSigned)
{
    pods::ResizableOutputBuffer out;
    testSignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testSignedRead(in);
}

TEST(resizeableMemoryStorage, testUnsigned)
{
    pods::ResizableOutputBuffer out;
    testUnsignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST(resizeableMemoryStorage, testFloat)
{
    pods::ResizableOutputBuffer out;
    testFloatWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testFloatRead(in);
}

TEST(resizeableMemoryStorage, testCharBool)
{
    pods::ResizableOutputBuffer out;
    testCharWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testCharRead(in);
}

TEST(resizeableMemoryStorage, testRawData)
{
    pods::ResizableOutputBuffer out;
    testRawDataWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testRawDataRead(in);
}

TEST(resizeableMemoryStorage, testError)
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
