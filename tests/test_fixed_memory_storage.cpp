#include <gtest/gtest.h>

#include <memory>

#include <serializationpp/memory_storage.h>

#include "storage_data.cpp"

class fixedSizeMemoryStorage
    : public ::testing::Test
{
    char data_[1024];
protected:
    spp::FixedSizeWriteOnlyMemoryStorage out { data_, sizeof(data_) };
};

TEST_F(fixedSizeMemoryStorage, testSigned)
{
    testSignedWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testSignedRead(in);
}

TEST_F(fixedSizeMemoryStorage, testUnsigned)
{
    testUnsignedWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST_F(fixedSizeMemoryStorage, testFloat)
{
    testFloatWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testFloatRead(in);
}

TEST_F(fixedSizeMemoryStorage, testCharBool)
{
    testCharBoolWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testCharBoolRead(in);
}

TEST_F(fixedSizeMemoryStorage, testRawData)
{
    testRawDataWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testRawDataRead(in);
}

TEST_F(fixedSizeMemoryStorage, testEnum)
{
    testEnumWrite(out);

    spp::ReadOnlyMemoryStorage in(out.data(), out.size());
    testEnumRead(in);
}

TEST_F(fixedSizeMemoryStorage, testError)
{
    uint16_t small = 0;
    uint64_t big = 0;

    {
        char data[4];
        spp::FixedSizeWriteOnlyMemoryStorage out(data, 4);
        EXPECT_FALSE(out.put(big));
    }

    {
        char data[sizeof(small)];
        spp::FixedSizeWriteOnlyMemoryStorage out(data, sizeof(small));
        EXPECT_TRUE(out.put(small));

        spp::ReadOnlyMemoryStorage in(out.data(), out.size());
        EXPECT_FALSE(in.get(big));
    }
}
