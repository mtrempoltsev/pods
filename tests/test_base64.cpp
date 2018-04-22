#include <gtest/gtest.h>

#include <array>

#include <pods/details/base64.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>

TEST(base64, common)
{
    const std::vector<char> data1 = { '1' };
    const std::vector<char> data2 = { '1', '2' };
    const std::vector<char> data3 = { '1', '2', '3' };
    const std::vector<char> data4 = { '1', '2', '3', '4' };
    const std::vector<char> data5 = { 41, 35, -66 };

    EXPECT_EQ(0u, pods::details::getBase64EncodedSize(0));

    EXPECT_NO_FATAL_FAILURE(pods::details::base64Encode(nullptr, 0));

    const auto encoded1 = pods::details::base64Encode(data1.data(), data1.size());
    EXPECT_EQ(encoded1, "MQ==");
    EXPECT_EQ(encoded1.size(), pods::details::getBase64EncodedSize(data1.size()));

    const auto encoded2 = pods::details::base64Encode(data2.data(), data2.size());
    EXPECT_EQ(encoded2, "MTI=");
    EXPECT_EQ(encoded2.size(), pods::details::getBase64EncodedSize(data2.size()));

    const auto encoded3 = pods::details::base64Encode(data3.data(), data3.size());
    EXPECT_EQ(encoded3, "MTIz");
    EXPECT_EQ(encoded3.size(), pods::details::getBase64EncodedSize(data3.size()));

    const auto encoded4 = pods::details::base64Encode(data4.data(), data4.size());
    EXPECT_EQ(encoded4, "MTIzNA==");
    EXPECT_EQ(encoded4.size(), pods::details::getBase64EncodedSize(data4.size()));

    const auto encoded5 = pods::details::base64Encode(data5.data(), data5.size());
    EXPECT_EQ(encoded5, "KSO+");
    EXPECT_EQ(encoded5.size(), pods::details::getBase64EncodedSize(data5.size()));

    EXPECT_NO_FATAL_FAILURE(pods::details::base64Decode(nullptr, 0, nullptr));
    EXPECT_EQ(0, pods::details::getBase64DecodedSize(nullptr, 0));

    ASSERT_EQ(data1.size(), pods::details::getBase64DecodedSize(encoded1.data(), encoded1.size()));
    std::vector<char> decoded1(data1.size());
    EXPECT_NO_FATAL_FAILURE(pods::details::base64Decode(encoded1.data(), encoded1.size(), decoded1.data()));
    EXPECT_EQ(decoded1, data1);

    ASSERT_EQ(data2.size(), pods::details::getBase64DecodedSize(encoded2.data(), encoded2.size()));
    std::vector<char> decoded2(data2.size());
    EXPECT_NO_FATAL_FAILURE(pods::details::base64Decode(encoded2.data(), encoded2.size(), decoded2.data()));
    EXPECT_EQ(decoded2, data2);

    ASSERT_EQ(data3.size(), pods::details::getBase64DecodedSize(encoded3.data(), encoded3.size()));
    std::vector<char> decoded3(data3.size());
    EXPECT_NO_FATAL_FAILURE(pods::details::base64Decode(encoded3.data(), encoded3.size(), decoded3.data()));
    EXPECT_EQ(decoded3, data3);

    ASSERT_EQ(data4.size(), pods::details::getBase64DecodedSize(encoded4.data(), encoded4.size()));
    std::vector<char> decoded4(data4.size());
    EXPECT_NO_FATAL_FAILURE(pods::details::base64Decode(encoded4.data(), encoded4.size(), decoded4.data()));
    EXPECT_EQ(decoded4, data4);
}
