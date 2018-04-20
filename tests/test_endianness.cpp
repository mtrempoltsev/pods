#include <gtest/gtest.h>

#include <pods/details/endianness.h>

#ifdef PODS_BIG_ENDIAN
    #define PODS_PASS pods::details::toBigEndian
    #define PODS_CONV pods::details::toLittleEndian
#else
    #define PODS_PASS pods::details::toLittleEndian
    #define PODS_CONV pods::details::toBigEndian
#endif

template <class T>
void check(T from, T expected)
{
    const auto a1 = PODS_PASS(from);
    EXPECT_EQ(from, a1);

    const auto a2 = PODS_CONV(from);
    EXPECT_EQ(expected, a2);

    const auto a3 = PODS_CONV(a2);
    EXPECT_EQ(from, a3);
}

TEST(testEndianess, common)
{
#ifdef PODS_BIG_ENDIAN
    EXPECT_TRUE(pods::details::BigEndian);
    EXPECT_FALSE(pods::details::LittleEndian);
#else
    EXPECT_TRUE(pods::details::LittleEndian);
    EXPECT_FALSE(pods::details::BigEndian);
#endif

    check<int16_t>(0x0102, 0x0201);
    check<uint16_t>(0x0102, 0x0201);
    check<int32_t>(0x01020304, 0x04030201);
    check<uint32_t>(0x01020304, 0x04030201);
    check<int64_t>(0x0102030405060708, 0x0807060504030201);
    check<uint64_t>(0x0102030405060708, 0x0807060504030201);
}

#undef PODS_PASS
#undef PODS_CONV
