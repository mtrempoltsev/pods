#include <gtest/gtest.h>

#include <pods/pods.h>
#include <pods/binary_serializer.h>
#include <pods/json_serializer.h>
#include <pods/buffers.h>

struct A1
{
    int x = 1;

    PODS_SERIALIZABLE(1, PODS_MDR(x))
};

struct A2
{
    int x = 3;
    int y = 5;

    static constexpr pods::Version version()
    {
        return 2;
    }

    template <class Serializer>
    pods::Error serialize(Serializer& serializer, pods::Version version)
    {
        if (version == 1)
        {
            return serializer(PODS_MDR(x));
        }

        return serializer(PODS_MDR(x), PODS_MDR(y));
    }
};

TEST(versioning, binary1)
{
    const A2 original;

    pods::ResizableOutputBuffer out;
    pods::BinarySerializer<decltype(out)> serializer(out);

    EXPECT_EQ(serializer.save(original), pods::Error::NoError);

    A1 actual;

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<decltype(in)> deserializer(in);

    EXPECT_EQ(deserializer.load(actual), pods::Error::ArchiveVersionMismatch);
}

TEST(versioning, json)
{
}
