#pragma once

#include "details/serializer.h"
#include "details/deserializer.h"

#include "details/formats/binary_input.h"
#include "details/formats/binary_output.h"

namespace pods
{
    template <class Storage>
    using BinarySerializer = details::Serializer<details::BinaryOutput<Storage>, Storage>;

    template <class Storage>
    using BinaryDeserializer = details::Deserializer<details::BinaryInput<Storage>, Storage>;
}
