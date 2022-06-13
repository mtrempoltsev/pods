#include <gtest/gtest.h>

#include <pods/buffers.h>
#include <pods/json.h>
#include <pods/msgpack.h>
#include <pods/pods.h>

struct Binary
{
    static constexpr int size = 3;

    std::vector<uint32_t> a; // zero size
    std::vector<uint32_t> b = { 0xfedcba98, 0x76543210, 0x648823fe };
    std::unique_ptr<int16_t[]> c;
    int16_t* d;
    std::array<uint32_t, 3> e = { { 0xfedcba98, 0x76543210, 0x648823fe } };
    std::array<uint8_t, 1> f = { { 100 } };
    uint64_t g[4] = { 1, 2, 3, 4 };
    std::vector<uint8_t> h;
    std::vector<uint8_t> i;

    static constexpr int hSize = 65535;
    static constexpr int iSize = 65536;

    void fill()
    {
        for (size_t j = 0; j < size; ++j)
        {
            c.get()[j] = static_cast<int16_t>(j + 1);
            d[j] = static_cast<int16_t>(j + 10);
        }

        uint8_t n = 0;

        for (int j = 0; j < hSize; ++j)
        {
            h.push_back(n++);
        }

        for (int j = 0; j < iSize; ++j)
        {
            i.push_back(n++);
        }
    }

    PODS_SERIALIZABLE(1, PODS_MDR_BIN(a), PODS_MDR_BIN(b), PODS_MDR_BIN_2(c, size),
        PODS_MDR_BIN_2(d, size), PODS_MDR_BIN(e), PODS_MDR_BIN(f), PODS_MDR_BIN(g),
        PODS_MDR_BIN(h), PODS_MDR_BIN(i))
};

template <class Serializer, class Deserializer>
void testBinary()
{
    int16_t buf1[Binary::size];
    int16_t buf2[Binary::size];

    Binary expected;
    expected.c = std::make_unique<int16_t[]>(expected.size);
    expected.d = buf1;

    expected.fill();

    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    EXPECT_EQ(serializer.save(expected), pods::Error::NoError);

    Binary actual{};
    actual.a.push_back(100);
    actual.b.clear();
    actual.c = std::make_unique<int16_t[]>(expected.size);
    std::fill_n(actual.c.get(), expected.size, static_cast<int16_t>(0));
    actual.d = buf2;
    std::fill_n(actual.d, expected.size, static_cast<int16_t>(0));
    std::fill_n(actual.e.data(), actual.e.size(), 0);
    std::fill_n(actual.f.data(), actual.f.size(), static_cast<uint8_t>(0));
    std::fill_n(actual.g, sizeof(actual.g) / sizeof(actual.g[0]), 0);

    pods::InputBuffer in(out.data(), out.size());
    Deserializer deserializer(in);
    EXPECT_EQ(deserializer.load(actual), pods::Error::NoError);

    EXPECT_EQ(expected.a, actual.a);
    EXPECT_EQ(expected.b, actual.b);

    for (size_t i = 0; i < expected.size; ++i)
    {
        EXPECT_EQ(expected.c.get()[i], actual.c.get()[i]);
        EXPECT_EQ(expected.d[i], actual.d[i]);
    }

    EXPECT_EQ(expected.e, actual.e);
    EXPECT_EQ(expected.f, actual.f);

    for (size_t i = 0; i < sizeof(actual.g) / sizeof(actual.g[0]); ++i)
    {
        EXPECT_EQ(expected.g[i], actual.g[i]);
    }

    EXPECT_EQ(expected.h[0], actual.h[0]);
    EXPECT_EQ(expected.h[Binary::hSize - 1], actual.h[Binary::hSize - 1]);

    EXPECT_EQ(expected.i[0], actual.i[0]);
    EXPECT_EQ(expected.i[Binary::hSize - 1], actual.i[Binary::hSize - 1]);
    EXPECT_EQ(expected.i[Binary::hSize], actual.i[Binary::hSize]);
    EXPECT_EQ(expected.i[Binary::iSize - 1], actual.i[Binary::iSize - 1]);
}

TEST(json, testBinary)
{
    testBinary<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(msgpack, testBinary)
{
    testBinary<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
