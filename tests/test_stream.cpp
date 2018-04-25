#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <sstream>
#include <vector>

#include <pods/streams.h>

TEST(stream, common)
{
    std::stringstream buffer;

    const uint16_t e1 = 1;
    const int8_t e2 = -2;
    const bool e3 = true;
    const bool e4 = false;
    const uint64_t e5 = 100;
    const int32_t e6 = 5000;
    const std::array<int32_t, 3> e7 = { { 1, 2, 3 } };

    pods::OutputStream out(buffer);

    EXPECT_EQ(out.put(e1), pods::Error::NoError);
    EXPECT_EQ(out.put(e2), pods::Error::NoError);
    EXPECT_EQ(out.put(e3), pods::Error::NoError);
    EXPECT_EQ(out.put(e4), pods::Error::NoError);
    EXPECT_EQ(out.put(e5), pods::Error::NoError);
    EXPECT_EQ(out.put(e6), pods::Error::NoError);
    EXPECT_EQ(out.put(e7.data(), e7.size()), pods::Error::NoError);

    out.flush();

    uint16_t a1 = 0;
    int8_t a2 = 0;
    bool a3 = false;
    bool a4 = true;
    uint64_t a5 = 0;
    int32_t a6 = 0;
    std::array<int32_t, 3> a7 = { { 0, 0, 0 } };

    pods::InputStream in(buffer);

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
    EXPECT_EQ(in.get(a6), pods::Error::NoError);
    EXPECT_EQ(e6, a6);
    EXPECT_EQ(in.get(a7.data(), a7.size()), pods::Error::NoError);
    EXPECT_EQ(e7, a7);

    EXPECT_EQ(in.get(a1), pods::Error::UnexpectedEnd);
}

TEST(stream, paging)
{
    std::stringstream buffer;

    const uint16_t e1 = 1;
    const int8_t e2 = -2;
    const bool e3 = true;
    const bool e4 = false;
    const uint64_t e5 = 100;
    const int32_t e6 = 5000;
    const std::array<int64_t, 9> e7 = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 } };
    const uint16_t e8 = 1;
    const int8_t e9 = -2;
    const uint64_t e10 = 100;
    const int32_t e11 = 5000;

    pods::OutputStream out(buffer, 8);

    EXPECT_EQ(out.put(e1), pods::Error::NoError);
    EXPECT_EQ(out.put(e2), pods::Error::NoError);
    EXPECT_EQ(out.put(e3), pods::Error::NoError);
    EXPECT_EQ(out.put(e4), pods::Error::NoError);
    EXPECT_EQ(out.put(e5), pods::Error::NoError);
    EXPECT_EQ(out.put(e6), pods::Error::NoError);
    EXPECT_EQ(out.put(e7.data(), e7.size()), pods::Error::NoError);
    EXPECT_EQ(out.put(e8), pods::Error::NoError);
    EXPECT_EQ(out.put(e9), pods::Error::NoError);
    EXPECT_EQ(out.put(e10), pods::Error::NoError);
    EXPECT_EQ(out.put(e11), pods::Error::NoError);

    out.flush();

    uint16_t a1 = 0;
    int8_t a2 = 0;
    bool a3 = false;
    bool a4 = true;
    uint64_t a5 = 0;
    int32_t a6 = 0;
    std::array<int64_t, 9> a7 = { { 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
    uint16_t a8 = 0;
    int8_t a9 = 0;
    uint64_t a10 = 0;
    int32_t a11 = 0;

    pods::InputStream in(buffer, 10);

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
    EXPECT_EQ(in.get(a6), pods::Error::NoError);
    EXPECT_EQ(e6, a6);
    EXPECT_EQ(in.get(a7.data(), a7.size()), pods::Error::NoError);
    EXPECT_EQ(e7, a7);
    EXPECT_EQ(in.get(a8), pods::Error::NoError);
    EXPECT_EQ(e8, a8);
    EXPECT_EQ(in.get(a9), pods::Error::NoError);
    EXPECT_EQ(e9, a9);
    EXPECT_EQ(in.get(a10), pods::Error::NoError);
    EXPECT_EQ(e10, a10);
    EXPECT_EQ(in.get(a11), pods::Error::NoError);
    EXPECT_EQ(e11, a11);

    EXPECT_EQ(in.get(a1), pods::Error::UnexpectedEnd);
}

TEST(stream, autoFlush)
{
    std::stringstream buffer;

    const uint16_t expected = 1;

    {
        pods::OutputStream out(buffer);
        EXPECT_EQ(out.put(expected), pods::Error::NoError);
    }

    uint16_t actual = 0;

    pods::InputStream in(buffer);
    EXPECT_EQ(in.get(actual), pods::Error::NoError);
    EXPECT_EQ(expected, actual);
}
