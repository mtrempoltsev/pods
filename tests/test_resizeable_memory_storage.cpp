#include <gtest/gtest.h>

#include <serialization++/memory_storage.h>

#include "storage_data.h"

TEST(resizeableMemoryStorage, testSigned)
{
    spp::ResizeableWriteOnlyMemoryStorage out;
    testSignedWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testSignedRead(in);
}

TEST(resizeableMemoryStorage, testUnsigned)
{
    spp::ResizeableWriteOnlyMemoryStorage out;
    testUnsignedWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST(resizeableMemoryStorage, testFloat)
{
    spp::ResizeableWriteOnlyMemoryStorage out;
    testFloatWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testFloatRead(in);
}

TEST(resizeableMemoryStorage, testCharBool)
{
    spp::ResizeableWriteOnlyMemoryStorage out;
    testCharBoolWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testCharBoolRead(in);
}

TEST(resizeableMemoryStorage, testRawData)
{
    spp::ResizeableWriteOnlyMemoryStorage out;
    testRawDataWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testRawDataRead(in);
}

TEST(resizeableMemoryStorage, testError)
{
    uint16_t small = 0;
    uint64_t big = 0;

    {
        spp::ResizeableWriteOnlyMemoryStorage out(4, 4);
        EXPECT_EQ(out.put(big), spp::Error::NotEnoughMemory);
    }

    {
        spp::ResizeableWriteOnlyMemoryStorage out;
        EXPECT_EQ(out.put(small), spp::Error::NoError);

        spp::ReadOnlyMemoryStorage in(out.data(), out.size());
        EXPECT_EQ(in.get(big), spp::Error::UnexpectedEnd);
    }
}
