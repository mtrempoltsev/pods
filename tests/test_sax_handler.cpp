#include <gtest/gtest.h>

#include <pods/details/formats/json_input.h>

TEST(saxHandler, startObject)
{
    pods::details::Handler<pods::details::StartObjectTag, pods::details::IsEndOfArrayData> handler;

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
    EXPECT_TRUE(handler.EndArray(2));
    EXPECT_TRUE(handler.data_.isEndOfArray_);
}

TEST(saxHandler, endObject)
{
    pods::details::Handler<pods::details::EndObjectTag, pods::details::EmptyData> handler;

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
    EXPECT_TRUE(handler.EndObject(1));
    EXPECT_FALSE(handler.StartArray());
    EXPECT_FALSE(handler.EndArray(2));
}

TEST(saxHandler, startArray)
{
    pods::details::Handler<pods::details::StartArrayTag, pods::details::IsEndOfArrayData> handler;

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
    EXPECT_TRUE(handler.EndArray(2));
    EXPECT_TRUE(handler.data_.isEndOfArray_);
}

TEST(saxHandler, endArray)
{
    pods::details::Handler<pods::details::EndArrayTag, pods::details::EmptyData> handler;

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

TEST(saxHandler, key)
{
    std::string key;

    pods::details::Handler<pods::details::KeyTag, pods::details::ValueAndIsEndOfArrayOrObjectData<std::string>> handler(key);

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
    EXPECT_FALSE(handler.data_.isEndOfArray_);
    EXPECT_FALSE(handler.data_.isEndOfObject_);
    EXPECT_TRUE(handler.EndObject(1));
    EXPECT_TRUE(handler.data_.isEndOfObject_);
    EXPECT_FALSE(handler.StartArray());
    EXPECT_TRUE(handler.EndArray(2));
    EXPECT_TRUE(handler.data_.isEndOfArray_);

    EXPECT_EQ(key, "key");
}

TEST(saxHandler, boolHandler)
{
    bool ok = false;
    pods::details::Handler<pods::details::BoolTag, pods::details::ValueAndIsEndOfArrayData<bool>> handler(ok);

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
    EXPECT_TRUE(handler.EndArray(2));
    EXPECT_TRUE(handler.data_.isEndOfArray_);

    EXPECT_TRUE(ok);
}

TEST(saxHandler, intHandler)
{
    {
        int32_t x = 0;
        pods::details::Handler<pods::details::IntTag, pods::details::ValueAndIsEndOfArrayData<decltype(x)>> handler(x);

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
        EXPECT_TRUE(handler.EndArray(2));
        EXPECT_TRUE(handler.data_.isEndOfArray_);

        EXPECT_TRUE(handler.Int(-5));
        EXPECT_EQ(x, -5);
    }

    {
        uint32_t x = 0;
        pods::details::Handler<pods::details::IntTag, pods::details::ValueAndIsEndOfArrayData<decltype(x)>> handler(x);

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
        EXPECT_TRUE(handler.EndArray(2));
        EXPECT_TRUE(handler.data_.isEndOfArray_);

        EXPECT_TRUE(handler.Int(5));
        EXPECT_EQ(x, 5u);
    }

    {
        int64_t x = 0;
        pods::details::Handler<pods::details::IntTag, pods::details::ValueAndIsEndOfArrayData<decltype(x)>> handler(x);

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
        EXPECT_TRUE(handler.EndArray(2));
        EXPECT_TRUE(handler.data_.isEndOfArray_);

        EXPECT_TRUE(handler.Int(-5));
        EXPECT_EQ(x, -5);
    }

    {
        uint64_t x = 0;
        pods::details::Handler<pods::details::IntTag, pods::details::ValueAndIsEndOfArrayData<decltype(x)>> handler(x);

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
        EXPECT_TRUE(handler.EndArray(2));
        EXPECT_TRUE(handler.data_.isEndOfArray_);

        EXPECT_TRUE(handler.Int(5));
        EXPECT_EQ(x, 5u);
    }
}

TEST(saxHandler, floating)
{
    {
        float x = 0;
        pods::details::Handler<pods::details::FloatTag, pods::details::ValueAndIsEndOfArrayData<decltype(x)>> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::lowest()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::max()));
        EXPECT_FALSE(handler.Double(std::numeric_limits<double>::lowest()));
        EXPECT_FALSE(handler.Double(std::numeric_limits<double>::max()));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_TRUE(handler.EndArray(2));
        EXPECT_TRUE(handler.data_.isEndOfArray_);

        EXPECT_TRUE(handler.Double(5.5));
        EXPECT_FLOAT_EQ(x, 5.5);
    }

    {
        double x = 0;
        pods::details::Handler<pods::details::FloatTag, pods::details::ValueAndIsEndOfArrayData<decltype(x)>> handler(x);

        EXPECT_FALSE(handler.Null());
        EXPECT_FALSE(handler.Bool(true));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::min()));
        EXPECT_FALSE(handler.Int(std::numeric_limits<int32_t>::max()));
        EXPECT_FALSE(handler.Uint(std::numeric_limits<uint32_t>::max()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::min()));
        EXPECT_FALSE(handler.Int64(std::numeric_limits<int64_t>::max()));
        EXPECT_FALSE(handler.Uint64(std::numeric_limits<uint64_t>::max()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::lowest()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<float>::max()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<double>::lowest()));
        EXPECT_TRUE(handler.Double(std::numeric_limits<double>::max()));
        EXPECT_FALSE(handler.RawNumber("15", 2, true));
        EXPECT_FALSE(handler.String("test", 4, true));
        EXPECT_FALSE(handler.StartObject());
        EXPECT_FALSE(handler.Key("key", 3, true));
        EXPECT_FALSE(handler.EndObject(1));
        EXPECT_FALSE(handler.StartArray());
        EXPECT_TRUE(handler.EndArray(2));
        EXPECT_TRUE(handler.data_.isEndOfArray_);

        EXPECT_TRUE(handler.Double(5.5));
        EXPECT_DOUBLE_EQ(x, 5.5);
    }
}

TEST(saxHandler, string)
{
    std::string x;
    pods::details::Handler<pods::details::StringTag, pods::details::ValueAndIsEndOfArrayData<decltype(x)>> handler(x);

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
    EXPECT_TRUE(handler.EndArray(2));
    EXPECT_TRUE(handler.data_.isEndOfArray_);

    EXPECT_EQ(x, "test");
}
