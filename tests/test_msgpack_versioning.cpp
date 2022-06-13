#include <gtest/gtest.h>

#include <pods/msgpack.h>
#include <pods/buffers.h>

#include "versioning_tests.h"

TEST(versioning, msgpackVersioningErr1)
{
    testErr1<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(versioning, msgpackVersioningErr2)
{
    testErr2<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(versioning, msgpackVersioningOk1)
{
    testOk1<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}

TEST(versioning, msgpackVersioningOk2)
{
    testOk2<pods::MsgPackSerializer<pods::ResizableOutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>();
}
