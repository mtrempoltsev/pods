#pragma once

#include "details/serializer.h"
#include "details/deserializer.h"

#include "details/formats/json_input.h"
#include "details/formats/json_output.h"

namespace pods
{
    template <class Storage>
    using JsonSerializer = details::Serializer<details::JsonOutput<Storage, rapidjson::Writer<details::OutputRapidJsonStreamWrapper<Storage>>>, Storage>;

    template <class Storage>
    using PrettyJsonSerializer = details::Serializer<details::JsonOutput<Storage, rapidjson::PrettyWriter<details::OutputRapidJsonStreamWrapper<Storage>>>, Storage>;

    template <class Storage>
    using JsonDeserializer = details::Deserializer<details::JsonInput<Storage>, Storage>;
}
