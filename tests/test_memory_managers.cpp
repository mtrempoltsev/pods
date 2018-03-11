#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include <pods/details/memory_managers.h>

TEST(fixedSizeMemoryManager, common)
{
    pods::details::FixedSizeMemoryManager m1(4);
    ASSERT_EQ(m1.available(), 4);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 2);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 0);
    ASSERT_TRUE(m1.getPtr(2) == nullptr);

    pods::details::FixedSizeMemoryManager m2(4);
    ASSERT_EQ(m2.available(), 4);
    ASSERT_TRUE(m2.getPtr(8) == nullptr);
    ASSERT_EQ(m2.available(), 4);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_EQ(m2.available(), 2);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_EQ(m2.available(), 0);
    ASSERT_TRUE(m2.getPtr(2) == nullptr);
    ASSERT_EQ(m2.available(), 0);
}

TEST(resizeableMemoryManager, common)
{
    pods::details::ResizeableMemoryManager m1(4, 4);
    ASSERT_EQ(m1.available(), 4);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 2);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 0);
    ASSERT_TRUE(m1.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m2(4, 4);
    ASSERT_TRUE(m2.getPtr(8) == nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m3(4, 8);
    ASSERT_EQ(m3.available(), 8);
    ASSERT_TRUE(m3.getPtr(8) != nullptr);
    ASSERT_EQ(m3.available(), 0);
    ASSERT_TRUE(m3.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m4(4, 16);
    ASSERT_EQ(m4.available(), 16);
    ASSERT_TRUE(m4.getPtr(8) != nullptr);
    ASSERT_EQ(m4.available(), 8);
    ASSERT_TRUE(m4.getPtr(4) != nullptr);
    ASSERT_EQ(m4.available(), 4);
    ASSERT_TRUE(m4.getPtr(8) == nullptr);
    ASSERT_EQ(m4.available(), 4);
    ASSERT_TRUE(m4.getPtr(4) != nullptr);
    ASSERT_EQ(m4.available(), 0);
    ASSERT_TRUE(m4.getPtr(1) == nullptr);

    const size_t n = 1000;
    const size_t bytes = n * sizeof(uint64_t);
    const size_t maxSize = bytes + 100;

    std::vector<uint64_t> expected;
    expected.reserve(n);

    uint64_t i = 0;
    std::generate_n(std::back_inserter(expected), n, [&i]() { return i++; });

    pods::details::ResizeableMemoryManager m5(4, maxSize);
    ASSERT_EQ(m5.available(), maxSize);

    auto ptr = m5.getPtr(bytes);
    ASSERT_EQ(m5.size(), bytes);
    ASSERT_EQ(m5.available(), maxSize - bytes);

    memcpy(ptr, expected.data(), bytes);

    std::vector<uint64_t> actual;
    actual.resize(n);

    memcpy(actual.data(), m5.data(), m5.size());

    EXPECT_EQ(expected, actual);
}

TEST(resizeableMemoryManager, canNotAllocate)
{
    const auto InvalidSize = std::numeric_limits<uint64_t>::max();

    EXPECT_THROW(pods::details::ResizeableMemoryManager m1(InvalidSize, InvalidSize), std::bad_alloc);

    const auto InitialSize = 4;

    pods::details::ResizeableMemoryManager m2(InitialSize, InvalidSize);

    EXPECT_FALSE(m2.getPtr(InvalidSize - InitialSize));
}

TEST(resizeableMemoryManager, move)
{
    pods::details::ResizeableMemoryManager m1(4, 8);

    const auto begin = m1.data();

    ASSERT_EQ(m1.getPtr(2), begin);

    pods::details::ResizeableMemoryManager m2 = std::move(m1);

    ASSERT_EQ(m1.data(), nullptr);

    ASSERT_EQ(m2.data(), begin);
    ASSERT_EQ(m2.size(), 2);
    ASSERT_EQ(m2.available(), 6);

    ASSERT_EQ(m1.getPtr(4), nullptr);

    ASSERT_EQ(m2.getPtr(4), begin + 2);

    ASSERT_EQ(m2.data(), begin);
    ASSERT_EQ(m2.size(), 6);
    ASSERT_EQ(m2.available(), 2);
}
