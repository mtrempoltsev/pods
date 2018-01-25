#include <gtest/gtest.h>

#include <array>

#include <pods/details/memory_managers.h>

TEST(fixedSizeMemoryManager, common)
{
    pods::details::FixedSizeMemoryManager m1(4);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_TRUE(m1.getPtr(2) == nullptr);

    pods::details::FixedSizeMemoryManager m2(4);
    ASSERT_TRUE(m2.getPtr(8) == nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) == nullptr);
}

TEST(resizeableMemoryManager, common)
{
    pods::details::ResizeableMemoryManager m1(4, 4);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_TRUE(m1.getPtr(2) != nullptr);
    ASSERT_TRUE(m1.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m2(4, 4);
    ASSERT_TRUE(m2.getPtr(8) == nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) != nullptr);
    ASSERT_TRUE(m2.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m3(4, 8);
    ASSERT_TRUE(m3.getPtr(8) != nullptr);
    ASSERT_TRUE(m3.getPtr(2) == nullptr);

    pods::details::ResizeableMemoryManager m4(4, 16);
    ASSERT_TRUE(m4.getPtr(8) != nullptr);
    ASSERT_TRUE(m4.getPtr(4) != nullptr);
    ASSERT_TRUE(m4.getPtr(8) == nullptr);
    ASSERT_TRUE(m4.getPtr(4) != nullptr);
    ASSERT_TRUE(m4.getPtr(1) == nullptr);

    const size_t n = 1000;
    const size_t bytes = n * sizeof(uint64_t);

    std::vector<uint64_t> expected;
    expected.reserve(n);

    uint64_t i = 0;
    std::generate_n(std::back_inserter(expected), n, [&i]() { return i++; });

    pods::details::ResizeableMemoryManager m5(4, bytes * 2);
    auto ptr = m5.getPtr(bytes);
    ASSERT_EQ(m5.size(), bytes);

    memcpy(ptr, expected.data(), bytes);

    std::vector<uint64_t> actual;
    actual.resize(n);

    memcpy(actual.data(), m5.data(), m5.size());

    EXPECT_EQ(expected, actual);
}
