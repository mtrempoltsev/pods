#include <gtest/gtest.h>

#include <memory>

#include <pods/buffers.h>

#include "storage_data.h"

class buffer_f
    : public ::testing::Test
{
protected:
    pods::OutputBuffer out { 1024 };
};

TEST_F(buffer_f, testInfo)
{
    const size_t totalSize = 1024;
    size_t size = 0;
    size_t available = 1024;

    EXPECT_EQ(out.available(), available);
    EXPECT_EQ(out.size(), size);

    const uint64_t n = 0;

    EXPECT_EQ(out.put(n), pods::Error::NoError);

    available -= sizeof(n);
    size += sizeof(n);

    EXPECT_EQ(out.available(), available);
    EXPECT_EQ(out.size(), size);

    out.clear();
    available = totalSize;
    size = 0;

    EXPECT_EQ(out.available(), available);
    EXPECT_EQ(out.size(), size);

    EXPECT_EQ(out.put(n), pods::Error::NoError);

    available -= sizeof(n);
    size += sizeof(n);

    EXPECT_EQ(out.available(), available);
    EXPECT_EQ(out.size(), size);

    out.clear();

    EXPECT_EQ(size % sizeof(n), 0);

    for (size_t i = 0; i < totalSize / sizeof(n); ++i)
    {
        EXPECT_EQ(out.put(n), pods::Error::NoError);
    }

    EXPECT_EQ(out.available(), 0);
    EXPECT_EQ(out.size(), totalSize);

    EXPECT_EQ(out.put(n), pods::Error::NotEnoughMemory);

    EXPECT_EQ(out.put(static_cast<uint8_t>(1)), pods::Error::NotEnoughMemory);
    EXPECT_EQ(out.available(), 0);
    EXPECT_EQ(out.size(), totalSize);

    EXPECT_EQ(out.put(static_cast<uint32_t>(1)), pods::Error::NotEnoughMemory);
    EXPECT_EQ(out.available(), 0);
    EXPECT_EQ(out.size(), totalSize);

    EXPECT_EQ(out.put(static_cast<bool>(1)), pods::Error::NotEnoughMemory);
    EXPECT_EQ(out.available(), 0);
    EXPECT_EQ(out.size(), totalSize);

    EXPECT_EQ(out.put(nullptr, 16), pods::Error::NotEnoughMemory);
    EXPECT_EQ(out.available(), 0);
    EXPECT_EQ(out.size(), totalSize);

    out.clear();
    available = totalSize;
    size = 0;

    EXPECT_EQ(out.available(), available);
    EXPECT_EQ(out.size(), size);

    char buf[totalSize - 1];

    EXPECT_EQ(out.put(buf, totalSize - 1), pods::Error::NoError);
    EXPECT_EQ(out.available(), 1);
    EXPECT_EQ(out.size(), totalSize - 1);

    EXPECT_EQ(out.put(static_cast<uint32_t>(1)), pods::Error::NotEnoughMemory);
    EXPECT_EQ(out.available(), 1);
    EXPECT_EQ(out.size(), totalSize - 1);

    EXPECT_EQ(out.put(static_cast<uint8_t>(1)), pods::Error::NoError);
    EXPECT_EQ(out.available(), 0);
    EXPECT_EQ(out.size(), totalSize);
}

TEST_F(buffer_f, testSigned)
{
    testSignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testSignedRead(in);
}

TEST_F(buffer_f, testUnsigned)
{
    testUnsignedWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testUnsignedRead(in);
}

TEST_F(buffer_f, testFloat)
{
    testFloatWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testFloatRead(in);
}

TEST_F(buffer_f, testCharBool)
{
    testBoolWrite(out);
    testCharWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testBoolRead(in);
    testCharRead(in);
}

TEST_F(buffer_f, testRawData)
{
    testRawDataWrite(out);

    pods::InputBuffer in(out.data(), out.size());
    testRawDataRead(in);
}

TEST(buffer, testError1)
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

TEST(buffer, testError2)
{
    uint8_t v1 = 10;
    pods::OutputBuffer out(1);
    EXPECT_EQ(out.put(v1), pods::Error::NoError);

    bool v2 = false;
    pods::InputBuffer in(out.data(), out.size());
    EXPECT_EQ(in.get(v2), pods::Error::CorruptedArchive);
}

TEST(buffer, testError3)
{
    const char v1[2] = { 0, 1 };
    pods::OutputBuffer out(2);
    EXPECT_EQ(out.put(v1, 2), pods::Error::NoError);

    char v2[3] = {};
    pods::InputBuffer in(out.data(), out.size());
    EXPECT_EQ(in.get(v2, 3), pods::Error::UnexpectedEnd);
}

TEST(buffer, testError4)
{
    const char v1[2] = { 0, 1 };
    pods::OutputBuffer out(1);
    EXPECT_EQ(out.put(v1, 2), pods::Error::NotEnoughMemory);
}

TEST(buffer, testError5)
{
    pods::OutputBuffer out(1);
    EXPECT_EQ(out.put(true), pods::Error::NoError);
    EXPECT_EQ(out.put(true), pods::Error::NotEnoughMemory);
}

TEST(buffer, testCtor)
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
