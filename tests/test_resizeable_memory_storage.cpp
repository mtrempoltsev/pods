#include <gtest/gtest.h>

#include <pods/memory_storage.h>

#include "storage_data.h"

TEST(resizeableMemoryStorage, testSigned)
{
    pods::ResizeableWriteOnlyMemoryStorage out;
    testSignedWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testSignedRead(in);
}

TEST(resizeableMemoryStorage, testUnsigned)
{
    pods::ResizeableWriteOnlyMemoryStorage out;
    testUnsignedWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST(resizeableMemoryStorage, testFloat)
{
    pods::ResizeableWriteOnlyMemoryStorage out;
    testFloatWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testFloatRead(in);
}

TEST(resizeableMemoryStorage, testCharBool)
{
    pods::ResizeableWriteOnlyMemoryStorage out;
    testCharWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testCharRead(in);
}

TEST(resizeableMemoryStorage, testRawData)
{
    pods::ResizeableWriteOnlyMemoryStorage out;
    testRawDataWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testRawDataRead(in);
}

TEST(resizeableMemoryStorage, testError)
{
    uint16_t small = 0;
    uint64_t big = 0;

    {
        pods::ResizeableWriteOnlyMemoryStorage out(4, 4);
        EXPECT_EQ(out.put(big), pods::Error::NotEnoughMemory);
    }

    {
        pods::ResizeableWriteOnlyMemoryStorage out;
        EXPECT_EQ(out.put(small), pods::Error::NoError);

        pods::ReadOnlyMemoryStorage in(out.data(), out.size());
        EXPECT_EQ(in.get(big), pods::Error::UnexpectedEnd);
    }
}
