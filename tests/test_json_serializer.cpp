#include <gtest/gtest.h>

#include <pods/json_serializer.h>
#include <pods/buffers.h>
#include <pods/streams.h>

#include "data.h"

TEST(jsonSerializer, common)
{
    const TestData expected;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    TestData actual;

    actual.x = 0;
    actual.y = 0;
    actual.z = std::string();
    actual.a.floatData = { { 0, 0, 0, 0, 0 } };
    actual.b.c.b = true;
    actual.b.c.e1 = AA;
    actual.b.c.e2 = Enum2::Y;
    actual.points.clear();
    actual.dict.clear();
    actual.strings.clear();
    actual.arr = { { 0, 0 } };
    actual.map.clear();

    const std::string json = std::string(out.data(), out.size());

    const std::string expectedJson = "{\"version\":1,\"x\":100,\"y\":200.5,\"z\":\"hello!\",\"a\":{\"version\":5,\"floatData\":[1.100000023841858,2.200000047683716,3.299999952316284,4.400000095367432,5.5]},\"b\":{\"version\":2,\"c\":{\"version\":1,\"b\":false,\"e1\":1,\"e2\":0}},\"points\":{\"version\":1,\"data\":[{\"x\":1,\"y\":2},{\"x\":3,\"y\":4}]},\"dict\":[{\"key\":-5,\"value\":[]},{\"key\":1,\"value\":[97,98]}],\"strings\":[\"hello\",\"мир\"],\"arr\":{\"version\":1,\"data\":[{\"x\":5,\"y\":6}]},\"map\":[{\"key\":{\"version\":1,\"x\":7,\"y\":8},\"value\":{\"version\":1,\"x\":9,\"y\":10}}]}";

    EXPECT_EQ(expectedJson, json);

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
    EXPECT_EQ(expected.y, actual.y);
    EXPECT_EQ(expected.z, actual.z);
    EXPECT_EQ(expected.a.floatData, actual.a.floatData);
    EXPECT_EQ(expected.b.c.b, actual.b.c.b);
    EXPECT_EQ(expected.b.c.e1, actual.b.c.e1);
    EXPECT_EQ(expected.b.c.e2, actual.b.c.e2);
    EXPECT_EQ(expected.points, actual.points);
    EXPECT_EQ(expected.dict, actual.dict);
    EXPECT_EQ(expected.strings, actual.strings);
    EXPECT_EQ(expected.arr, actual.arr);
    EXPECT_EQ(expected.map, actual.map);
}

TEST(jsonSerializer, error)
{
    const TestData expected;
    pods::OutputBuffer out(128);
    pods::PrettyJsonSerializer<pods::OutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::WriteError);
}

struct InnerData
{
    uint32_t z;
    uint32_t a;

    PODS_SERIALIZABLE(
        2,
        PODS_MDR(z),
        PODS_MDR(a))
};

struct OptionalTestData
{
    uint32_t x;
    uint32_t y;

    InnerData z;

    PODS_SERIALIZABLE(
        1,
        PODS_OPT(x),
        PODS_MDR(y),
        PODS_OPT(z))
};

TEST(jsonSerializer, optionalFields1)
{
    const std::string json = "{\"version\": 1,\"x\": 7,\"y\": 8,\"z\":{\"version\":2,\"z\":9,\"a\":5}}";

    OptionalTestData data = {};

    pods::InputBuffer in(json.c_str(), json.length());

    pods::JsonDeserializer<decltype(in)> serializer(in);
    EXPECT_EQ(serializer.load(data), pods::Error::NoError);

    EXPECT_EQ(data.x, 7);
    EXPECT_EQ(data.y, 8);
    EXPECT_EQ(data.z.z, 9);
    EXPECT_EQ(data.z.a, 5);
}

TEST(jsonSerializer, optionalFields2)
{
    const std::string json = "{\"version\": 1,\"y\": 8}";

    OptionalTestData data = {};

    pods::InputBuffer in(json.c_str(), json.length());

    pods::JsonDeserializer<decltype(in)> serializer(in);
    EXPECT_EQ(serializer.load(data), pods::Error::NoError);

    EXPECT_EQ(data.x, 0);
    EXPECT_EQ(data.y, 8);
    EXPECT_EQ(data.z.z, 0);
}

TEST(jsonSerializer, optionalFields3)
{
    const std::string json = "{\"version\": 1,\"x\": 7}";

    OptionalTestData data = {};

    pods::InputBuffer in(json.c_str(), json.length());

    pods::JsonDeserializer<decltype(in)> serializer(in);
    EXPECT_EQ(serializer.load(data), pods::Error::MandatoryFieldMissed);
}

TEST(jsonSerializer, optionalFields4)
{
    const std::string json = "{\"version\": 1,\"x\": 7,\"y\": 8,\"z\":{\"version\":2,\"a\":5}}";

    OptionalTestData data = {};

    pods::InputBuffer in(json.c_str(), json.length());

    pods::JsonDeserializer<decltype(in)> serializer(in);
    EXPECT_EQ(serializer.load(data), pods::Error::MandatoryFieldMissed);
}

TEST(jsonSerializer, binary)
{
    const BinData1 expected;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData1 actual;
    std::fill(actual.x.begin(), actual.x.end(), 0);

    const std::string json = "{\"version\":1,\"x\":\"mLrc/hAyVHb+I4hk\"}";

    EXPECT_EQ(std::string(out.data(), out.size()), json);

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
}

TEST(jsonSerializer, binary2)
{
    int16_t buf1[BinData2::size];
    int16_t buf2[BinData2::size];

    const BinData2 expected = { std::make_unique<int16_t[]>(expected.size), buf1 };

    for (size_t i = 0; i < expected.size; ++i)
    {
        expected.x.get()[i] = static_cast<int16_t>(i + 1);
        expected.y[i] = static_cast<int16_t>(i + 10);
    }

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData2 actual = { std::make_unique<int16_t[]>(expected.size), buf2 };
    std::fill_n(actual.x.get(), expected.size, 0);
    std::fill_n(actual.y, expected.size, 0);

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    for (size_t i = 0; i < expected.size; ++i)
    {
        EXPECT_EQ(actual.x.get()[i], i + 1);
        EXPECT_EQ(actual.y[i], i + 10);
    }
}

TEST(jsonSerializer, binaryLarger)
{
    const BinData1 expected;

    const std::string json = "{\"version\":1,\"x\":\"mLrc/hAyVHb+I4hk\"}";

    BinData1 actual;
    actual.x.clear();

    pods::InputBuffer in(json.data(), json.length());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
}

TEST(jsonSerializer, binaryLargerArray)
{
    const Array1 expected;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Array2 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::CorruptedArchive);
}

TEST(jsonSerializer, binarySmallerArray)
{
    const Array2 expected;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Array1 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::CorruptedArchive);
}

TEST(jsonSerializer, stream)
{
    const char* json =
        R"({
            "version": 1,
            "a": 100,
            "z": 300
        })";

    InnerData actual = {};

    std::stringstream stream(json);
    pods::InputStream in(stream);
    pods::JsonDeserializer<pods::InputStream> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);
    EXPECT_EQ(actual.a, 100);
    EXPECT_EQ(actual.z, 300);
}

TEST(jsonSerializer, cArray)
{
    const CArray expected;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    CArray actual = {};

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x[0], actual.x[0]);
    EXPECT_EQ(expected.x[1], actual.x[1]);
    EXPECT_EQ(expected.x[2], actual.x[2]);
    EXPECT_EQ(expected.x[3], actual.x[3]);
}

TEST(jsonSerializer, cArrayBin)
{
    const CArrayBin expected;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    const std::string expectedJson = "{\"version\":1,\"x\":\"AQAAAAIAAAADAAAABAAAAA==\",\"ok\":true}";
    const std::string actualJson(out.data(), out.size());
    EXPECT_EQ(expectedJson, actualJson);

    CArrayBin actual = {};

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x[0], actual.x[0]);
    EXPECT_EQ(expected.x[1], actual.x[1]);
    EXPECT_EQ(expected.x[2], actual.x[2]);
    EXPECT_EQ(expected.x[3], actual.x[3]);
}

TEST(jsonSerializer, errors)
{
    const InvalidSize expected;

    pods::ResizableOutputBuffer out;
    pods::JsonSerializer<pods::ResizableOutputBuffer> serializer(out);

    EXPECT_EQ(serializer.save(expected), pods::Error::SizeToLarge);
}
