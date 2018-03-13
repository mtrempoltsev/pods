#include <gtest/gtest.h>

#include <memory>

#include <pods/buffers.h>

#include "storage_data.h"

class buffer
    : public ::testing::Test
{
protected:
    pods::OutputBuffer out { 1024 };
};

TEST_F(buffer, testSigned)
{
    testSignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testSignedRead(in);
}

TEST_F(buffer, testUnsigned)
{
    testUnsignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST_F(buffer, testFloat)
{
    testFloatWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testFloatRead(in);
}

TEST_F(buffer, testCharBool)
{
    testBoolWrite(out);
    testCharWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testBoolRead(in);
    testCharRead(in);
}

TEST_F(buffer, testRawData)
{
    testRawDataWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testRawDataRead(in);
}

TEST_F(buffer, testError1)
{
    uint16_t small = 0;
    uint64_t big = 0;

    {
        pods::OutputBuffer out(4);
        EXPECT_EQ(out.put(big), pods::Error::NotEnoughMemory);
    }

    {
        pods::OutputBuffer out(sizeof(small));
        EXPECT_EQ(out.put(small), pods::Error::NoError);

        pods::InputBuffer in(out.data(), out.size());
        EXPECT_EQ(in.get(big), pods::Error::UnexpectedEnd);
    }
}

TEST_F(buffer, testError2)
{
    uint8_t v1 = 10;
    pods::OutputBuffer out(1);
    EXPECT_EQ(out.put(v1), pods::Error::NoError);

    bool v2 = false;
    pods::InputBuffer in(out.data(), out.size());
    EXPECT_EQ(in.get(v2), pods::Error::CorruptedArchive);
}

TEST_F(buffer, testError3)
{
    const char v1[2] = { 0, 1 };
    pods::OutputBuffer out(2);
    EXPECT_EQ(out.put(v1, 2), pods::Error::NoError);

    char v2[3] = {};
    pods::InputBuffer in(out.data(), out.size());
    EXPECT_EQ(in.get(v2, 3), pods::Error::UnexpectedEnd);
}

TEST_F(buffer, testError4)
{
    const char v1[2] = { 0, 1 };
    pods::OutputBuffer out(1);
    EXPECT_EQ(out.put(v1, 2), pods::Error::NotEnoughMemory);
}

TEST_F(buffer, testError5)
{
    pods::OutputBuffer out(1);
    EXPECT_EQ(out.put(true), pods::Error::NoError);
    EXPECT_EQ(out.put(true), pods::Error::NotEnoughMemory);
}

TEST_F(buffer, testCtor)
{
    const uint16_t expected = 5;
    char buf[sizeof(expected)] = {};
    pods::OutputBuffer out(buf, sizeof(expected));
    EXPECT_EQ(out.put(expected), pods::Error::NoError);

    uint16_t actual = 0;
    pods::InputBuffer in(out.data(), out.size());
    EXPECT_EQ(in.get(actual), pods::Error::NoError);

    EXPECT_EQ(expected, actual);
}
