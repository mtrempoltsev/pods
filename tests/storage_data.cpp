#include <limits>
#include <string>

const int8_t i8expected1 = std::numeric_limits<int8_t>::min();
const int8_t i8expected2 = std::numeric_limits<int8_t>::max();
const int16_t i16expected1 = std::numeric_limits<int16_t>::min();
const int16_t i16expected2 = std::numeric_limits<int16_t>::max();
const int32_t i32expected1 = std::numeric_limits<int32_t>::min();
const int32_t i32expected2 = std::numeric_limits<int32_t>::max();
const int64_t i64expected1 = std::numeric_limits<int64_t>::min();
const int64_t i64expected2 = std::numeric_limits<int64_t>::max();

const uint8_t ui8expected1 = std::numeric_limits<uint8_t>::min();
const uint8_t ui8expected2 = std::numeric_limits<uint8_t>::max();
const uint16_t ui16expected1 = std::numeric_limits<uint16_t>::min();
const uint16_t ui16expected2 = std::numeric_limits<uint16_t>::max();
const uint32_t ui32expected1 = std::numeric_limits<uint32_t>::min();
const uint32_t ui32expected2 = std::numeric_limits<uint32_t>::max();
const uint64_t ui64expected1 = std::numeric_limits<uint64_t>::min();
const uint64_t ui64expected2 = std::numeric_limits<uint64_t>::max();

const float f1expected = std::numeric_limits<float>::min();
const float f2expected = std::numeric_limits<float>::max();
const double d1expected = std::numeric_limits<double>::min();
const double d2expected = std::numeric_limits<double>::max();

const char c1expected = std::numeric_limits<char>::min();
const char c2expected = std::numeric_limits<char>::max();
const bool b1expected = std::numeric_limits<bool>::min();
const bool b2expected = std::numeric_limits<bool>::max();

const std::string data1expected = "hello";
const std::string data2expected = "@#0123456789-=AaBbCcDdEeFf!";

enum First
{
    A,
    B
};

enum class Second : uint64_t
{
    X,
    Y = 0xfffffffffffffff0
};

const First enum1expected = A;
const Second enum2expected = Second::Y;

template <class WriteOnlyStorage>
void testSignedWrite(WriteOnlyStorage& out)
{
    EXPECT_TRUE(out.put(i8expected1));
    EXPECT_TRUE(out.put(i8expected2));
    EXPECT_TRUE(out.put(i16expected1));
    EXPECT_TRUE(out.put(i16expected2));
    EXPECT_TRUE(out.put(i32expected1));
    EXPECT_TRUE(out.put(i32expected2));
    EXPECT_TRUE(out.put(i64expected1));
    EXPECT_TRUE(out.put(i64expected2));
}

template <class ReadOnlyStorage>
void testSignedRead(ReadOnlyStorage& in)
{
    int8_t i8actual1 = 0;
    EXPECT_TRUE(in.get(i8actual1));
    EXPECT_EQ(i8actual1, i8expected1);

    int8_t i8actual2 = 0;
    EXPECT_TRUE(in.get(i8actual2));
    EXPECT_EQ(i8actual2, i8expected2);

    int16_t i16actual1 = 0;
    EXPECT_TRUE(in.get(i16actual1));
    EXPECT_EQ(i16actual1, i16expected1);

    int16_t i16actual2 = 0;
    EXPECT_TRUE(in.get(i16actual2));
    EXPECT_EQ(i16actual2, i16expected2);

    int32_t i32actual1 = 0;
    EXPECT_TRUE(in.get(i32actual1));
    EXPECT_EQ(i32actual1, i32expected1);

    int32_t i32actual2 = 0;
    EXPECT_TRUE(in.get(i32actual2));
    EXPECT_EQ(i32actual2, i32expected2);

    int64_t i64actual1 = 0;
    EXPECT_TRUE(in.get(i64actual1));
    EXPECT_EQ(i64actual1, i64expected1);

    int64_t i64actual2 = 0;
    EXPECT_TRUE(in.get(i64actual2));
    EXPECT_EQ(i64actual2, i64expected2);
}

template <class WriteOnlyStorage>
void testUnsignedWrite(WriteOnlyStorage& out)
{
    EXPECT_TRUE(out.put(ui8expected1));
    EXPECT_TRUE(out.put(ui8expected2));
    EXPECT_TRUE(out.put(ui16expected1));
    EXPECT_TRUE(out.put(ui16expected2));
    EXPECT_TRUE(out.put(ui32expected1));
    EXPECT_TRUE(out.put(ui32expected2));
    EXPECT_TRUE(out.put(ui64expected1));
    EXPECT_TRUE(out.put(ui64expected2));
}

template <class ReadOnlyStorage>
void testUnsignedRead(ReadOnlyStorage& in)
{
    uint8_t ui8actual1 = 0;
    EXPECT_TRUE(in.get(ui8actual1));
    EXPECT_EQ(ui8actual1, ui8expected1);

    uint8_t ui8actual2 = 0;
    EXPECT_TRUE(in.get(ui8actual2));
    EXPECT_EQ(ui8actual2, ui8expected2);

    uint16_t ui16actual1 = 0;
    EXPECT_TRUE(in.get(ui16actual1));
    EXPECT_EQ(ui16actual1, ui16expected1);

    uint16_t ui16actual2 = 0;
    EXPECT_TRUE(in.get(ui16actual2));
    EXPECT_EQ(ui16actual2, ui16expected2);

    uint32_t ui32actual1 = 0;
    EXPECT_TRUE(in.get(ui32actual1));
    EXPECT_EQ(ui32actual1, ui32expected1);

    uint32_t ui32actual2 = 0;
    EXPECT_TRUE(in.get(ui32actual2));
    EXPECT_EQ(ui32actual2, ui32expected2);

    uint64_t ui64actual1 = 0;
    EXPECT_TRUE(in.get(ui64actual1));
    EXPECT_EQ(ui64actual1, ui64expected1);

    uint64_t ui64actual2 = 0;
    EXPECT_TRUE(in.get(ui64actual2));
    EXPECT_EQ(ui64actual2, ui64expected2);
}

template <class WriteOnlyStorage>
void testFloatWrite(WriteOnlyStorage& out)
{
    EXPECT_TRUE(out.put(f1expected));
    EXPECT_TRUE(out.put(f2expected));
    EXPECT_TRUE(out.put(d1expected));
    EXPECT_TRUE(out.put(d2expected));
}

template <class ReadOnlyStorage>
void testFloatRead(ReadOnlyStorage& in)
{
    float f1actual = 0;
    EXPECT_TRUE(in.get(f1actual));
    EXPECT_EQ(f1actual, f1expected);

    float f2actual = 0;
    EXPECT_TRUE(in.get(f2actual));
    EXPECT_EQ(f2actual, f2expected);

    double d1actual = 0;
    EXPECT_TRUE(in.get(d1actual));
    EXPECT_EQ(d1actual, d1expected);

    double d2actual = 0;
    EXPECT_TRUE(in.get(d2actual));
    EXPECT_EQ(d2actual, d2expected);
}

template <class WriteOnlyStorage>
void testCharBoolWrite(WriteOnlyStorage& out)
{
    EXPECT_TRUE(out.put(c1expected));
    EXPECT_TRUE(out.put(c2expected));
    EXPECT_TRUE(out.put(b1expected));
    EXPECT_TRUE(out.put(b2expected));
}

template <class ReadOnlyStorage>
void testCharBoolRead(ReadOnlyStorage& in)
{
    char c1actual = 0;
    EXPECT_TRUE(in.get(c1actual));
    EXPECT_EQ(c1actual, c1expected);

    char c2actual = 0;
    EXPECT_TRUE(in.get(c2actual));
    EXPECT_EQ(c2actual, c2expected);

    bool b1actual = 0;
    EXPECT_TRUE(in.get(b1actual));
    EXPECT_EQ(b1actual, b1expected);

    bool b2actual = 0;
    EXPECT_TRUE(in.get(b2actual));
    EXPECT_EQ(b2actual, b2expected);
}

template <class WriteOnlyStorage>
void testRawDataWrite(WriteOnlyStorage& out)
{
    EXPECT_TRUE(out.put(data1expected.c_str(), data1expected.size()));
    EXPECT_TRUE(out.put(data2expected.c_str(), data2expected.size()));
}

template <class ReadOnlyStorage>
void testRawDataRead(ReadOnlyStorage& in)
{
    std::string data1actual(data1expected.size(), '\0');
    EXPECT_TRUE(in.get(&*data1actual.begin(), data1actual.size()));
    EXPECT_EQ(data1actual, data1expected);

    std::string data2actual(data2expected.size(), '\0');
    EXPECT_TRUE(in.get(&*data2actual.begin(), data2actual.size()));
    EXPECT_EQ(data2actual, data2expected);
}

template <class WriteOnlyStorage>
void testEnumWrite(WriteOnlyStorage& out)
{
    EXPECT_TRUE(out.put(enum1expected));
    EXPECT_TRUE(out.put(enum2expected));
}

template <class ReadOnlyStorage>
void testEnumRead(ReadOnlyStorage& in)
{
    First enuim1actual = B;
    EXPECT_TRUE(in.get(enuim1actual));
    EXPECT_EQ(enuim1actual, enum1expected);

    Second enuim2actual = Second::X;
    EXPECT_TRUE(in.get(enuim2actual));
    EXPECT_EQ(enuim2actual, enum2expected);
}
