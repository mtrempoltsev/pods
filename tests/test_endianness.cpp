#include <gtest/gtest.h>

#include <pods/details/endianness.h>

#ifdef PODS_BIG_ENDIAN
    #define PODS_PASS_TO pods::details::toBigEndian
    #define PODS_PASS_FROM pods::details::fromBigEndian
    #define PODS_CONV_TO pods::details::toLittleEndian
    #define PODS_CONV_FROM pods::details::fromLittleEndian
#else
    #define PODS_PASS_TO pods::details::toLittleEndian
    #define PODS_PASS_FROM pods::details::fromLittleEndian
    #define PODS_CONV_TO pods::details::toBigEndian
    #define PODS_CONV_FROM pods::details::fromBigEndian
#endif

template <class T>
void check(T value, T mirror)
{
    const auto a1 = PODS_PASS_TO(value);
    EXPECT_EQ(value, a1);

    const auto a2 = PODS_PASS_FROM(a1);
    EXPECT_EQ(value, a2);

    const auto a3 = PODS_CONV_TO(value);
    EXPECT_EQ(mirror, a3);

    const auto a4 = PODS_CONV_FROM(a3);
    EXPECT_EQ(value, a4);
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

    check<int8_t>(0x01, 0x01);
    check<uint8_t>(0x01, 0x01);
    check<int16_t>(0x0102, 0x0201);
    check<uint16_t>(0x0102, 0x0201);
    check<int32_t>(0x01020304, 0x04030201);
    check<uint32_t>(0x01020304, 0x04030201);
    check<int64_t>(0x0102030405060708, 0x0807060504030201);
    check<uint64_t>(0x0102030405060708, 0x0807060504030201);
}

#undef PODS_PASS_TO
#undef PODS_PASS_FROM
#undef PODS_CONV_TO
#undef PODS_CONV_FROM
