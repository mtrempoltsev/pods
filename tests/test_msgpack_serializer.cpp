#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/msgpack.h>
#include <pods/streams.h>

#include "serialization_tests.h"

struct Int8
{
    using Type = int8_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct UInt8
{
    using Type = uint8_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct Int16
{
    using Type = int16_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct UInt16
{
    using Type = uint16_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct Int32
{
    using Type = int32_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct UInt32
{
    using Type = uint32_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct Int64
{
    using Type = int64_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct UInt64
{
    using Type = uint64_t;
    Type x;
    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

template <class FromT, class ToT, class T>
void check(T expected)
{
    const FromT from { static_cast<typename FromT::Type>(expected) };

    pods::ResizableOutputBuffer out;
    pods::MsgPackSerializer<decltype(out)> serializer(out);
    EXPECT_EQ(serializer.save(from), pods::Error::NoError);

    ToT to;

    pods::InputBuffer in(out.data(), out.size());
    pods::MsgPackDeserializer<decltype(in)> deserializer(in);
    EXPECT_EQ(deserializer.load(to), pods::Error::NoError);

    EXPECT_EQ(expected, to.x);
}

TEST(msgpackSerializer, testBool)
{
    testBool<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(msgpackSerializer, testInt)
{
    testInt<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(msgpackSerializer, testUnsigned)
{
    testUnsigned<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(msgpackSerializer, testFloat)
{
    testFloat<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(msgpackSerializer, testString)
{
    testString<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(msgpackSerializer, testBinary)
{
    testBinary<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(msgpackSerializer, testIntSizes)
{
    const auto min5bits = -32;

    const auto min1byte = std::numeric_limits<int8_t>::min();
    const auto max1byte = std::numeric_limits<int8_t>::max();

    const auto min2byte = std::numeric_limits<int16_t>::min();
    const auto max2byte = std::numeric_limits<int16_t>::max();

    const auto min4byte = std::numeric_limits<int32_t>::min();
    const auto max4byte = std::numeric_limits<int32_t>::max();

    check<Int16, Int8>(min5bits);
    check<Int16, Int8>(min1byte);
    check<Int16, Int8>(max1byte);

    check<Int32, Int8>(min5bits);
    check<Int32, Int8>(min1byte);
    check<Int32, Int8>(max1byte);
    check<Int32, Int16>(min2byte);
    check<Int32, Int16>(max2byte);

    check<Int64, Int8>(min5bits);
    check<Int64, Int8>(min1byte);
    check<Int64, Int8>(max1byte);
    check<Int64, Int16>(min2byte);
    check<Int64, Int16>(max2byte);
    check<Int64, Int32>(min4byte);
    check<Int64, Int32>(max4byte);
}

TEST(msgpackSerializer, testUIntSizes)
{
    const auto max7bits = 127u;
    const auto max1byte = std::numeric_limits<uint8_t>::max();
    const auto max2byte = std::numeric_limits<uint16_t>::max();
    const auto max4byte = std::numeric_limits<uint32_t>::max();

    check<UInt16, UInt8>(max7bits);
    check<UInt16, UInt8>(max1byte);

    check<UInt32, UInt8>(max7bits);
    check<UInt32, UInt8>(max1byte);
    check<UInt32, UInt16>(max2byte);

    check<UInt64, UInt8>(max7bits);
    check<UInt64, UInt8>(max1byte);
    check<UInt64, UInt16>(max2byte);
    check<UInt64, UInt32>(max4byte);
}

TEST(msgpackSerializer, testArray)
{
    testArray<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(msgpackSerializer, testMap)
{
    testMap<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
