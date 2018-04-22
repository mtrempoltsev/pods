#include <gtest/gtest.h>

#include <pods/details/rapidjson_wrappers.h>
#include <pods/buffers.h>

TEST(rapidJsonWrapper, common)
{
    pods::OutputBuffer out(2);

    pods::details::OutputRapidJsonStreamWrapper<decltype(out)> outWrapper(out);

    EXPECT_TRUE(outWrapper.good());
    outWrapper.Put('a');
    EXPECT_TRUE(outWrapper.good());
    outWrapper.Put('b');
    EXPECT_TRUE(outWrapper.good());
    outWrapper.Put('c');
    EXPECT_FALSE(outWrapper.good());

    pods::InputBuffer in(out.data(), out.size());

    pods::details::InputRapidJsonStreamWrapper<decltype(in)> inWrapper(in);

    EXPECT_TRUE(inWrapper.good());
    EXPECT_EQ(inWrapper.Peek(), 'a');
    EXPECT_EQ(inWrapper.Tell(), 0u);
    EXPECT_EQ(inWrapper.Take(), 'a');
    EXPECT_TRUE(inWrapper.good());
    EXPECT_EQ(inWrapper.Peek(), 'b');
    EXPECT_EQ(inWrapper.Tell(), 1u);
    EXPECT_EQ(inWrapper.Take(), 'b');
    EXPECT_TRUE(inWrapper.good());
    EXPECT_EQ(inWrapper.Peek(), '\0');
    EXPECT_EQ(inWrapper.Tell(), 2u);
    EXPECT_EQ(inWrapper.Take(), '\0');
    EXPECT_FALSE(inWrapper.good());
}
