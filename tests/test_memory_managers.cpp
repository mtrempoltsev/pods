#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include <pods/details/memory_managers.h>

TEST(fixedSizeMemoryManager, common)
{
    pods::details::FixedSizeMemoryManager m1(4);
    ASSERT_EQ(m1.available(), 4u);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 2u);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 0u);
    ASSERT_TRUE(m1.getPtr(2) == nullptr);

    pods::details::FixedSizeMemoryManager m2(4);
    ASSERT_EQ(m2.available(), 4u);
    ASSERT_TRUE(m2.getPtr(8) == nullptr);
    ASSERT_EQ(m2.available(), 4u);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_EQ(m2.available(), 2u);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_EQ(m2.available(), 0u);
    ASSERT_TRUE(m2.getPtr(2) == nullptr);
    ASSERT_EQ(m2.available(), 0u);
}

TEST(resizeableMemoryManager, common)
{
    pods::details::ResizeableMemoryManager m1(4, 4);
    ASSERT_EQ(m1.available(), 4u);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 2u);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_EQ(m1.available(), 0u);
    ASSERT_TRUE(m1.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m2(4, 4);
    ASSERT_TRUE(m2.getPtr(8) == nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m3(4, 8);
    ASSERT_EQ(m3.available(), 8u);
    ASSERT_TRUE(m3.getPtr(8) != nullptr);
    ASSERT_EQ(m3.available(), 0u);
    ASSERT_TRUE(m3.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m4(4, 16);
    ASSERT_EQ(m4.available(), 16u);
    ASSERT_TRUE(m4.getPtr(8) != nullptr);
    ASSERT_EQ(m4.available(), 8u);
    ASSERT_TRUE(m4.getPtr(4) != nullptr);
    ASSERT_EQ(m4.available(), 4u);
    ASSERT_TRUE(m4.getPtr(8) == nullptr);
    ASSERT_EQ(m4.available(), 4u);
    ASSERT_TRUE(m4.getPtr(4) != nullptr);
    ASSERT_EQ(m4.available(), 0u);
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

TEST(resizeableMemoryManager, canNotAllocate1)
{
    const size_t InvalidSize = std::numeric_limits<uint64_t>::max() / 4;

    try
    {
        pods::details::ResizeableMemoryManager m(InvalidSize, InvalidSize);
        EXPECT_EQ(m.getPtr(4), nullptr);
    }
    catch (const std::bad_alloc&)
    {
        EXPECT_TRUE(true);
    }
}

TEST(resizeableMemoryManager, canNotAllocate2)
{
    const size_t InvalidSize = std::numeric_limits<uint64_t>::max() / 4;
    const size_t InitialSize = 4;

    pods::details::ResizeableMemoryManager m(InitialSize, InvalidSize);

    EXPECT_FALSE(m.getPtr(InvalidSize - InitialSize));
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
    ASSERT_EQ(m2.available(), 6u);

    ASSERT_EQ(m1.getPtr(4), nullptr);

    ASSERT_TRUE(m2.getPtr(4) != nullptr);
    ASSERT_TRUE(m2.data() != nullptr);
    ASSERT_EQ(m2.size(), 6);
    ASSERT_EQ(m2.available(), 2u);
}
