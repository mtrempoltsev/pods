#include <gtest/gtest.h>

#include <pods/json.h>
#include <pods/buffers.h>

#include "versioning_tests.h"

TEST(versioning, jsonVersioningErr1)
{
    testErr1<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(versioning, jsonVersioningErr2)
{
    testErr2<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(versioning, jsonVersioningOk1)
{
    testOk1<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}

TEST(versioning, jsonVersioningOk2)
{
    testOk2<pods::JsonSerializer<pods::ResizableOutputBuffer>, pods::JsonDeserializer<pods::InputBuffer>>();
}
