#include <gtest/gtest.h>

#include <pods/binary.h>
#include <pods/buffers.h>
#include <pods/streams.h>

#include "data.h"

TEST(binarySerializer, common)
{
    const TestData expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
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
    actual.arr = { { { 0, 0 } } };
    actual.map.clear();

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
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

TEST(binarySerializer, binary1)
{
    const BinData1 expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData1 actual;
    std::fill(actual.x.begin(), actual.x.end(), 0);

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
}

TEST(binarySerializer, binary2)
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
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData2 actual = { std::make_unique<int16_t[]>(expected.size), buf2 };
    std::fill_n(actual.x.get(), expected.size, 0);
    std::fill_n(actual.y, expected.size, 0);

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    for (size_t i = 0; i < expected.size; ++i)
    {
        EXPECT_EQ(actual.x.get()[i], static_cast<int16_t>(i + 1));
        EXPECT_EQ(actual.y[i], static_cast<int16_t>(i + 10));
    }
}

TEST(binarySerializer, binaryLarger)
{
    const BinData1 expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData1 actual;
    actual.x.clear();

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
}

TEST(binarySerializer, binaryLargerArray)
{
    const Array1 expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Array2 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::CorruptedArchive);
}

TEST(binarySerializer, binarySmallerArray)
{
    const Array2 expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Array1 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::CorruptedArchive);
}

TEST(binarySerializer, stream)
{
    const BinData1 expected;

    std::stringstream buf;

    pods::OutputStream out(buf);
    pods::BinarySerializer<decltype(out)> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    BinData1 actual;
    actual.x.clear();

    pods::InputStream in(buf);
    pods::BinaryDeserializer<decltype(in)> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x, actual.x);
}

TEST(binarySerializer, cArray)
{
    const CArray expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    CArray actual;
    actual.ok = false;
    std::fill_n(actual.x, 4, 0);

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x[0], actual.x[0]);
    EXPECT_EQ(expected.x[1], actual.x[1]);
    EXPECT_EQ(expected.x[2], actual.x[2]);
    EXPECT_EQ(expected.x[3], actual.x[3]);
}

TEST(binarySerializer, cArrayBin)
{
    const CArrayBin expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    CArrayBin actual = {};

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<pods::InputBuffer> deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.x[0], actual.x[0]);
    EXPECT_EQ(expected.x[1], actual.x[1]);
    EXPECT_EQ(expected.x[2], actual.x[2]);
    EXPECT_EQ(expected.x[3], actual.x[3]);
}

TEST(binarySerializer, errors)
{
    const InvalidSize expected;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<pods::ResizableOutputBuffer> serializer(out);

    EXPECT_EQ(serializer.save(expected), pods::Error::SizeToLarge);
}
