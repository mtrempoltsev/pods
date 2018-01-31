#include <gtest/gtest.h>

#include <memory>

#include <pods/memory_storage.h>

#include "storage_data.h"

class fixedSizeMemoryStorage
    : public ::testing::Test
{
protected:
    pods::FixedSizeWriteOnlyMemoryStorage out { 1024 };
};

TEST_F(fixedSizeMemoryStorage, testSigned)
{
    testSignedWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testSignedRead(in);
}

TEST_F(fixedSizeMemoryStorage, testUnsigned)
{
    testUnsignedWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST_F(fixedSizeMemoryStorage, testFloat)
{
    testFloatWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testFloatRead(in);
}

TEST_F(fixedSizeMemoryStorage, testCharBool)
{
    testCharWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testCharRead(in);
}

TEST_F(fixedSizeMemoryStorage, testRawData)
{
    testRawDataWrite(out);

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    testRawDataRead(in);
}

TEST_F(fixedSizeMemoryStorage, testError)
{
    uint16_t small = 0;
    uint64_t big = 0;

    {
        pods::FixedSizeWriteOnlyMemoryStorage out1(4);
        EXPECT_EQ(out1.put(big), pods::Error::NotEnoughMemory);
    }

    {
        pods::FixedSizeWriteOnlyMemoryStorage out2(sizeof(small));
        EXPECT_EQ(out2.put(small), pods::Error::NoError);

        pods::ReadOnlyMemoryStorage in(out2.data(), out2.size());
        EXPECT_EQ(in.get(big), pods::Error::UnexpectedEnd);
    }
}
