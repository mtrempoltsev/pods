#include <gtest/gtest.h>

#include <pods/details/formats/json_input.h>

TEST(saxHandlers, startObject)
{
    pods::details::StartObjectHandler handler;

    EXPECT_FALSE(handler.Null());
    EXPECT_FALSE(handler.Bool(true));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
    EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
    EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
    EXPECT_FALSE(handler.Double(0.5));
    EXPECT_FALSE(handler.RawNumber("15", 2, true));
    EXPECT_FALSE(handler.String("test", 4, true));
    EXPECT_TRUE(handler.StartObject());
    EXPECT_FALSE(handler.Key("key", 3, true));
    EXPECT_FALSE(handler.EndObject(1));
    EXPECT_FALSE(handler.StartArray());
    EXPECT_FALSE(handler.EndArray(2));
}

TEST(saxHandlers, startArray)
{
    pods::details::StartArrayHandler handler;

    EXPECT_FALSE(handler.Null());
    EXPECT_FALSE(handler.Bool(true));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
    EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
    EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
    EXPECT_FALSE(handler.Double(0.5));
    EXPECT_FALSE(handler.RawNumber("15", 2, true));
    EXPECT_FALSE(handler.String("test", 4, true));
    EXPECT_FALSE(handler.StartObject());
    EXPECT_FALSE(handler.Key("key", 3, true));
    EXPECT_FALSE(handler.EndObject(1));
    EXPECT_TRUE(handler.StartArray());
    EXPECT_FALSE(handler.EndArray(2));
}

TEST(saxHandlers, endArray)
{
    pods::details::EndArrayHandler handler;

    EXPECT_FALSE(handler.Null());
    EXPECT_FALSE(handler.Bool(true));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
    EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
    EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
    EXPECT_FALSE(handler.Double(0.5));
    EXPECT_FALSE(handler.RawNumber("15", 2, true));
    EXPECT_FALSE(handler.String("test", 4, true));
    EXPECT_FALSE(handler.StartObject());
    EXPECT_FALSE(handler.Key("key", 3, true));
    EXPECT_FALSE(handler.EndObject(1));
    EXPECT_FALSE(handler.StartArray());
    EXPECT_TRUE(handler.EndArray(2));
}

TEST(saxHandlers, key)
{
    {
        std::string next;
        bool isEndOfObject = false;

        pods::details::KeyHandler handler("key", next, isEndOfObject);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_FALSE(handler.Double(0.5));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_TRUE(handler.Key("key", 3, true));
        EXPECT_FALSE(isEndOfObject);
        EXPECT_TRUE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));

        EXPECT_TRUE(next.empty());
        EXPECT_TRUE(isEndOfObject);
    }

    {
        std::string next;
        bool isEndOfObject = false;

        pods::details::KeyHandler handler("no", next, isEndOfObject);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_FALSE(handler.Double(0.5));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_TRUE(handler.Key("key", 3, true));
        EXPECT_FALSE(isEndOfObject);
        EXPECT_TRUE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));

        EXPECT_EQ(next, "key");
        EXPECT_TRUE(isEndOfObject);
    }
}

TEST(saxHandlers, boolHandler)
{
    bool ok = false;
    pods::details::BoolHandler handler(ok);

    EXPECT_FALSE(handler.Null());
    EXPECT_TRUE(handler.Bool(true));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
    EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
    EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
    EXPECT_FALSE(handler.Double(0.5));
    EXPECT_FALSE(handler.RawNumber("15", 2, true));
    EXPECT_FALSE(handler.String("test", 4, true));
    EXPECT_FALSE(handler.StartObject());
    EXPECT_FALSE(handler.Key("key", 3, true));
    EXPECT_FALSE(handler.EndObject(1));
    EXPECT_FALSE(handler.StartArray());
    EXPECT_FALSE(handler.EndArray(2));

    EXPECT_TRUE(ok);
}

TEST(saxHandlers, intHandler)
{
    {
        int32_t x = 0;
        pods::details::IntHandler<decltype(x)> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_TRUE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_TRUE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_FALSE(handler.Double(0.5));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));
    }

    {
        uint32_t x = 0;
        pods::details::IntHandler<decltype(x)> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_TRUE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_TRUE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_FALSE(handler.Double(0.5));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));
    }

    {
        int64_t x = 0;
        pods::details::IntHandler<decltype(x)> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_TRUE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_TRUE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_TRUE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_TRUE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_TRUE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_FALSE(handler.Double(0.5));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));
    }

    {
        uint64_t x = 0;
        pods::details::IntHandler<decltype(x)> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_TRUE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_TRUE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_TRUE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_TRUE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_FALSE(handler.Double(0.5));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));
    }
}

TEST(saxHandlers, floating)
{
    {
        float x = 0;
        pods::details::FloatingHandler<decltype(x)> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::min()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::max()));
        EXPECT_FALSE(handler.Double(std::numeric_limits<double>::min()));
        EXPECT_FALSE(handler.Double(std::numeric_limits<double>::max()));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));
    }

    {
        double x = 0;
        pods::details::FloatingHandler<decltype(x)> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::min()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::max()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<double>::min()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<double>::max()));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_FALSE(handler.EndArray(2));
    }
}

TEST(saxHandlers, string)
{
    std::string x;
    pods::details::StringHandler handler(x);

    EXPECT_FALSE(handler.Null());
    EXPECT_FALSE(handler.Bool(true));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
    EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
    EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
    EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
    EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
    EXPECT_FALSE(handler.Double(0.5));
    EXPECT_FALSE(handler.RawNumber("15", 2, true));
    EXPECT_TRUE(handler.String("test", 4, true));
    EXPECT_FALSE(handler.StartObject());
    EXPECT_FALSE(handler.Key("key", 3, true));
    EXPECT_FALSE(handler.EndObject(1));
    EXPECT_FALSE(handler.StartArray());
    EXPECT_FALSE(handler.EndArray(2));

    EXPECT_EQ(x, "test");
}
