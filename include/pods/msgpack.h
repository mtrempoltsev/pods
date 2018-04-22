#pragma once

#include "details/serializer.h"
#include "details/deserializer.h"

#include "details/formats/msgpack_input.h"
#include "details/formats/msgpack_output.h"

namespace pods
{
    template <class Storage>
    using MsgPackSerializer = details::Serializer<details::MsgPackOutput<Storage>, Storage>;

    template <class Storage>
    using MsgPackDeserializer = details::Deserializer<details::MsgPackInput<Storage>, Storage>;
}
