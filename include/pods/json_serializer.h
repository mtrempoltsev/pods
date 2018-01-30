#pragma once

#include <array>
#include <map>
#include <type_traits>
#include <vector>

#include <rapidjson/rapidjson.h>
#include <rapidjson/writer.h>

#include "details/utils.h"

#include "errors.h"
#include "types.h"

namespace pods
{
    static constexpr char PODS_VERSION_STR[] = "version";
    static constexpr char PODS_KEY_STR[] = "key";
    static constexpr char PODS_VALUE_STR[] = "value";
    static constexpr char PODS_DATA_STR[] = "data";

    template <class Storage>
    class JsonSerializer final
    {
        struct RapidJsonStreamWrapper
        {
            typedef char Ch;

            RapidJsonStreamWrapper(Storage& storage)
                : storage_(storage)
                , good_(true)
            {
            }

            void Put(Ch c)
            {
                if (storage_.put(c) == Error::NoError)
                {
                    return;
                }

                good_ = false;
            }

            void Flush() noexcept
            {
            }

            bool good() const noexcept
            {
                return good_;
            }

        private:
            Storage& storage_;
            bool good_;
        };

    public:
        explicit JsonSerializer(Storage& storage) noexcept
            : stream_(storage)
            , writer_(stream_)
        {
        }

        JsonSerializer(const JsonSerializer&) = delete;
        JsonSerializer& operator=(const JsonSerializer&) = delete;

        template <class T>
        Error save(T&& data)
        {
            return serialize(std::forward<T&&>(data));
        }

        Error operator()() noexcept
        {
            return Error::NoError;
        }

        template <class... T>
        Error operator()(T&&... args)
        {
            return process(std::forward<T>(args)...);
        }

    private:
        template <class T>
        Error serialize(const T& value)
        {
            PODS_SAFE_CALL(startObject());
            PODS_SAFE_CALL(writeVersion<T>());
            PODS_SAFE_CALL(const_cast<T&>(value).serialize(*this, T::version()));
            return endObject();
        }

        template <class T>
        Error serializeWithoutVersion(const T& value)
        {
            PODS_SAFE_CALL(startObject());
            PODS_SAFE_CALL(const_cast<T&>(value).serialize(*this, T::version()));
            return endObject();
        }

        template <class T>
        Error process(const char* name, const T& value)
        {
            return writeKeyValue(name, value);
        }

        template <class T, class... ArgsT>
        Error process(const char* name, const T& value, ArgsT&... args)
        {
            const auto error = writeKeyValue(name, value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        template <class T>
        Error writeKeyValue(const char* name, const T& value)
        {
            PODS_SAFE_CALL(writeKey(name));
            return writeValue(value);
        }

        template <class T>
        Error writeVersion()
        {
            static_assert(details::IsPodsSerializable<T>::value,
                "you must define the methods version() and serialize() for each serializable class or struct");
            PODS_SAFE_CALL(writeKey(PODS_VERSION_STR));
            return writeValue(std::decay_t<T>::version());
        }

        Error startArray()
        {
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

        Error writeKey(const char* key)
        {
            return writer_.Key(key) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        Error writeValue(bool value)
        {
            return writer_.Bool(value) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        Error writeValue(int32_t value)
        {
            return writer_.Int(value) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        Error writeValue(uint32_t value)
        {
            return writer_.Uint(value) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        Error writeValue(int64_t value)
        {
            return writer_.Int64(value) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        Error writeValue(uint64_t value)
        {
            return writer_.Uint64(value) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        Error writeValue(double value)
        {
            return writer_.Double(value) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        Error writeValue(T value)
        {
            return writeValue(std::underlying_type_t<T>(value));
        }

        Error writeValue(const std::string& value)
        {
            return writer_.String(value.c_str(), value.length()) && stream_.good()
                ? Error::NoError
                : Error::WriteError;
        }

        template <class T, typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error writeValue(const T& value)
        {
            return serialize(value);
        }

        template <class T, size_t Size>
        Error writeValue(const std::array<T, Size>& value)
        {
            return writeRange(value.data(), value.size());
        }

        template <class K, class V>
        Error writeValue(const std::map<K, V>& value)
        {
            PODS_SAFE_CALL(startArray());

            for (const auto& pair : value)
            {
                PODS_SAFE_CALL(startObject());
                PODS_SAFE_CALL(writeKey(PODS_KEY_STR));
                PODS_SAFE_CALL(writeValue(pair.first));
                PODS_SAFE_CALL(writeKey(PODS_VALUE_STR));
                PODS_SAFE_CALL(writeValue(pair.second));
                PODS_SAFE_CALL(endObject());
            }

            PODS_SAFE_CALL(endArray());
            return Error::NoError;
        }

        template <class T>
        Error writeValue(const std::vector<T>& value)
        {
            return writeRange(value.data(), value.size());
        }

        template <class T, typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error writeRange(T* begin, size_t size)
        {
            PODS_SAFE_CALL(startObject());
            PODS_SAFE_CALL(writeVersion<T>());
            PODS_SAFE_CALL(writeKey(PODS_DATA_STR));
            PODS_SAFE_CALL(startArray());

            for (auto end = begin + size; begin != end; ++begin)
            {
                PODS_SAFE_CALL(serializeWithoutVersion(*begin));
            }

            PODS_SAFE_CALL(endArray());
            return endObject();
        }

        template <class T, typename std::enable_if<std::is_class<T>::value && !details::IsPodsSerializable<T>::value, int>::type = 0>
        Error writeRange(T* begin, size_t size)
        {
            PODS_SAFE_CALL(startArray());

            for (auto end = begin + size; begin != end; ++begin)
            {
                PODS_SAFE_CALL(writeValue(*begin));
            }

            return endArray();
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error writeRange(T* begin, size_t size)
        {
            PODS_SAFE_CALL(startArray());

            for (auto end = begin + size; begin != end; ++begin)
            {
                PODS_SAFE_CALL(writeValue(*begin));
            }

            return endArray();
        }

    private:
        RapidJsonStreamWrapper stream_;
        rapidjson::Writer<RapidJsonStreamWrapper> writer_;
    };
}
