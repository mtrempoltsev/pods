#pragma once

#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>

#include "../base64.h"
#include "../rapidjson_wrappers.h"
#include "../utils.h"

#include "../../errors.h"
#include "../../types.h"

namespace pods
{
    namespace details
    {
        template <class Storage, class RapidjsonWriter>
        class JsonOutput final
        {
        public:
            explicit JsonOutput(Storage& storage) noexcept
                : stream_(storage)
                , writer_(stream_)
            {
            }

            JsonOutput(const JsonOutput<Storage, RapidjsonWriter>&) = delete;
            JsonOutput& operator=(const JsonOutput<Storage, RapidjsonWriter>&) = delete;

            Error startSerialization()
            {
                return startObject();
            }

            Error endSerialization()
            {
                PODS_SAFE_CALL(endObject());
                stream_.Flush();
                return Error::NoError;
            }

            Error saveName(const char* name)
            {
                return writer_.Key(getName(name)) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error startObject()
            {
                return writer_.StartObject() && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error endObject()
            {
                return writer_.EndObject() && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error startArray(Size size)
            {
                PODS_SAFE_CALL(saveName(PODS_SIZE));
                PODS_SAFE_CALL(save(size));
                PODS_SAFE_CALL(saveName(PODS_DATA));
                return writer_.StartArray() && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error endArray()
            {
                return writer_.EndArray() && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error startMap(Size size)
            {
                return startArray(size);
            }

            Error endMap()
            {
                return endArray();
            }

            Error save(int32_t value)
            {
                return writer_.Int(value) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error save(uint32_t value)
            {
                return writer_.Uint(value) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error save(int64_t value)
            {
                return writer_.Int64(value) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error save(uint64_t value)
            {
                return writer_.Uint64(value) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error save(double value)
            {
                return writer_.Double(value) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error save(bool value)
            {
                return writer_.Bool(value) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            Error save(const std::string& value)
            {
                return writer_.String(value.c_str(), static_cast<rapidjson::SizeType>(value.length())) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            template <class T>
            Error saveBlob(const T* data, Size size)
            {
                const auto base64 = details::base64Encode(data, size);
                return save(base64);
            }

        private:
            OutputRapidJsonStreamWrapper<Storage> stream_;
            RapidjsonWriter writer_;
        };
    }
}
