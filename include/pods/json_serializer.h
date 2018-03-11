#pragma once

#include <array>
#include <map>
#include <stack>
#include <type_traits>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>

#include "errors.h"
#include "types.h"

#include "details/base64.h"
#include "details/binary.h"
#include "details/rapidjson_wrappers.h"
#include "details/utils.h"

namespace pods
{
    namespace details
    {
        static constexpr char PODS_VERSION_STR[] = "version";
        static constexpr char PODS_KEY_STR[] = "key";
        static constexpr char PODS_VALUE_STR[] = "value";
        static constexpr char PODS_DATA_STR[] = "data";

        constexpr bool isOptional(const char* name) noexcept
        {
            return *name == 0;
        }

        constexpr const char* getName(const char* name) noexcept
        {
            return isOptional(name)
                ? name + 1
                : name;
        }

        template <class Storage, class RapidjsonWriter>
        class JsonSerializerImpl final
        {
        public:
            explicit JsonSerializerImpl(Storage& storage) noexcept
                : stream_(storage)
                , writer_(stream_)
            {
            }

            JsonSerializerImpl(const JsonSerializerImpl<Storage, RapidjsonWriter>&) = delete;
            JsonSerializerImpl& operator=(const JsonSerializerImpl<Storage, RapidjsonWriter>&) = delete;

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
            Error process(const char* name, const T& value, ArgsT&&... args)
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
                static_assert(IsPodsSerializable<T>::value,
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
                return writer_.Key(getName(key)) && stream_.good()
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
                return writer_.String(value.c_str(), static_cast<rapidjson::SizeType>(value.length())) && stream_.good()
                    ? Error::NoError
                    : Error::WriteError;
            }

            template <class T, typename std::enable_if<IsPodsSerializable<T>::value, int>::type = 0>
            Error writeValue(const T& value)
            {
                return serialize(value);
            }

            Error writeValue(const details::BinaryArray& value)
            {
                if (value.size() > std::numeric_limits<Size>::max())
                {
                    return Error::SizeToLarge;
                }

                const auto base64 = details::base64Encode(value.data(), value.size());
                return writeValue(base64);
            }

            template <class T>
            Error writeValue(const details::BinaryVector<T>& value)
            {
                if (value.size() > std::numeric_limits<Size>::max())
                {
                    return Error::SizeToLarge;
                }

                const auto base64 = details::base64Encode(value.data(), value.size());
                return writeValue(base64);
            }

            template <class T, size_t ArraySize>
            Error writeValue(const T(&value)[ArraySize])
            {
                return writeRange(value, ArraySize);
            }

            template <class T, size_t ArraySize>
            Error writeValue(const std::array<T, ArraySize>& value)
            {
                return writeRange(value.data(), ArraySize);
            }

            template <class K, class V>
            Error writeValue(const std::map<K, V>& value)
            {
                if (value.size() > std::numeric_limits<Size>::max())
                {
                    return Error::SizeToLarge;
                }

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
                if (value.size() > std::numeric_limits<Size>::max())
                {
                    return Error::SizeToLarge;
                }

                return writeRange(value.data(), static_cast<Size>(value.size()));
            }

            template <class T, typename std::enable_if<IsPodsSerializable<T>::value, int>::type = 0>
            Error writeRange(T* begin, Size size)
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

            template <class T, typename std::enable_if<std::is_class<T>::value && !IsPodsSerializable<T>::value, int>::type = 0>
            Error writeRange(T* begin, Size size)
            {
                PODS_SAFE_CALL(startArray());

                for (auto end = begin + size; begin != end; ++begin)
                {
                    PODS_SAFE_CALL(writeValue(*begin));
                }

                return endArray();
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
            Error writeRange(T* begin, Size size)
            {
                PODS_SAFE_CALL(startArray());

                for (auto end = begin + size; begin != end; ++begin)
                {
                    PODS_SAFE_CALL(writeValue(*begin));
                }

                return endArray();
            }

        private:
            OutputRapidJsonStreamWrapper<Storage> stream_;
            RapidjsonWriter writer_;
        };
    }

    template <class Storage>
    using JsonSerializer = details::JsonSerializerImpl<Storage, rapidjson::Writer<details::OutputRapidJsonStreamWrapper<Storage>>>;

    template <class Storage>
    using PrettyJsonSerializer = details::JsonSerializerImpl<Storage, rapidjson::PrettyWriter<details::OutputRapidJsonStreamWrapper<Storage>>>;

    template <class Storage>
    class JsonDeserializer final
    {
    public:
        explicit JsonDeserializer(Storage& storage)
        {
            details::InputRapidJsonStreamWrapper<Storage> stream(storage);
            document_.ParseStream(stream);
        }

        JsonDeserializer(const JsonDeserializer<Storage>&) = delete;
        JsonDeserializer& operator=(const JsonDeserializer<Storage>&) = delete;

        template <class T>
        Error load(T& data)
        {
            if (document_.HasParseError())
            {
                return Error::CorruptedArchive;
            }

            auto sentry = goDeeperAndBackOnExit(document_);

            return deserialize(data);
        }

        Error operator()() noexcept
        {
            return Error::NoError;
        }

        template <class... T>
        Error operator()(T&&... args)
        {
            return process(args...);
        }

    private:
        using Member = rapidjson::Document::MemberIterator;
        using Value = rapidjson::Document::ValueType;
        using Array = rapidjson::Document::ValueType::Array;

        template <class T>
        Error deserialize(T& value)
        {
            Version version = 0;
            PODS_SAFE_CALL(getVersion<T>(version));
            return value.serialize(*this, version);
        }

        template <class T>
        Error deserializeWithoutVersion(T& value, Version version)
        {
            return value.serialize(*this, version);
        }

        template <class T>
        Error process(const char* name, T& value)
        {
            return readValue(name, value);
        }

        template <class T, class... ArgsT>
        Error process(const char* name, T& value, ArgsT&... args)
        {
            const auto error = readValue(name, value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        template <class T>
        Error readValue(const char* name, T& value)
        {
            Member it;
            if (getMember(name, it) != Error::NoError)
            {
                return details::isOptional(name)
                    ? Error::NoError
                    : Error::MandatoryFieldMissed;
            }
            return read(it->value, value);
        }

        Error read(Value& data, bool& value)
        {
            if (data.IsBool())
            {
                value = data.GetBool();
                return Error::NoError;
            }
            return Error::CorruptedArchive;
        }

        template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        Error read(Value& data, T& value)
        {
            std::underlying_type_t<T> i = 0;
            PODS_SAFE_CALL(read(data, i));
            value = static_cast<T>(i);
            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
        Error read(Value& data, T& value)
        {
            if (data.IsDouble())
            {
                value = static_cast<T>(data.GetDouble());
                return Error::NoError;
            }
            return Error::CorruptedArchive;
        }

        template <class T, typename std::enable_if<std::is_signed<T>::value && !std::is_floating_point<T>::value, int>::type = 0>
        Error read(Value& data, T& value)
        {
            if (data.IsInt64())
            {
                const auto i = data.GetInt64();
                if (i < std::numeric_limits<T>::min() || i > std::numeric_limits<T>::max())
                {
                    return Error::CorruptedArchive;
                }

                value = static_cast<T>(i);
                return Error::NoError;
            }
            return Error::CorruptedArchive;
        }

        template <class T, typename std::enable_if<std::is_unsigned<T>::value, int>::type = 0>
        Error read(Value& data, T& value)
        {
            if (data.IsUint64())
            {
                const auto i = data.GetUint64();
                if (i > std::numeric_limits<T>::max())
                {
                    return Error::CorruptedArchive;
                }

                value = static_cast<T>(i);
                return Error::NoError;
            }
            return Error::CorruptedArchive;
        }

        Error read(Value& data, std::string& value)
        {
            if (data.IsString())
            {
                value = data.GetString();
                return Error::NoError;
            }
            return Error::CorruptedArchive;
        }

        template <class T, typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error read(Value& data, T& value)
        {
            if (data.IsObject())
            {
                auto sentry = goDeeperAndBackOnExit(data);
                PODS_SAFE_CALL(deserialize(value));
                return Error::NoError;
            }
            return Error::CorruptedArchive;
        }

        Error read(Value& data, details::BinaryArray& value)
        {
            if (data.IsString())
            {
                const auto encoded = data.GetString();
                const auto encodedSize = data.GetStringLength();
                const auto decodedSize = details::getBase64DecodedSize(encoded, encodedSize);
                if (decodedSize == value.size())
                {
                    details::base64Decode(encoded, encodedSize, value.data());
                    return Error::NoError;
                }
            }
            return Error::CorruptedArchive;
        }

        template <class T>
        Error read(Value& data, details::BinaryVector<T>& value)
        {
            if (data.IsString())
            {
                const auto encoded = data.GetString();
                const auto encodedSize = data.GetStringLength();
                const auto decodedSize = details::getBase64DecodedSize(encoded, encodedSize);

                PODS_SAFE_CALL(value.allocate(decodedSize));

                details::base64Decode(encoded, encodedSize, value.data());
                return Error::NoError;
            }
            return Error::CorruptedArchive;
        }

        template <class T, size_t ArraySize>
        Error readArray(Value& data, T* value)
        {
            return readRange<T, T*>(data,
                [&](Size size, T*& it)
                {
                    if (size == ArraySize)
                    {
                        it = value;
                        return Error::NoError;
                    }
                    return Error::CorruptedArchive;
                });
        }

        template <class T, size_t ArraySize>
        Error read(Value& data, T (&value)[ArraySize])
        {
            return readArray<T, ArraySize>(data, value);
        }

        template <class T, size_t ArraySize>
        Error read(Value& data, std::array<T, ArraySize>& value)
        {
            return readArray<T, ArraySize>(data, value.data());
        }

        template <class K, class V>
        Error read(Value& data, std::map<K, V>& value)
        {
            if (!data.IsArray())
            {
                return Error::CorruptedArchive;
            }

            const Array& array = data.GetArray();

            Size size = 0;
            PODS_SAFE_CALL(readSize(array, size));

            value.clear();

            auto hint = value.begin();
            for (Size i = 0; i < size; ++i)
            {
                auto& elem = array[i];

                if (!elem.IsObject())
                {
                    return Error::CorruptedArchive;
                }

                auto sentry = goDeeperAndBackOnExit(elem);

                K key;
                PODS_SAFE_CALL(readValue(details::PODS_KEY_STR, key));

                V val;
                PODS_SAFE_CALL(readValue(details::PODS_VALUE_STR, val));

                hint = value.emplace_hint(hint, std::move(key), std::move(val));
            }

            return Error::NoError;
        }

        template <class T>
        Error read(Value& data, std::vector<T>& value)
        {
            return readRange<T, T*>(data,
                [&value](Size size, T*& it)
                {
                    value.resize(size);
                    it = value.data();
                    return Error::NoError;
                });
        }

        Error getMember(const char* name, rapidjson::Document::MemberIterator& it)
        {
            auto node = nodes_.top();
            it = node->FindMember(details::getName(name));
            return it != node->MemberEnd()
                ? Error::NoError
                : Error::CorruptedArchive;
        }

        Error readSize(const Array& array, Size& size)
        {
            const auto n = array.Size();
            if (n > std::numeric_limits<Size>::max())
            {
                return Error::CorruptedArchive;
            }

            size = static_cast<Size>(n);

            return Error::NoError;
        }

        template <class T, class Iterator, class InitFunction,
            typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error readRange(Value& data, InitFunction init)
        {
            if (!data.IsObject())
            {
                return Error::CorruptedArchive;
            }

            auto sentry = goDeeperAndBackOnExit(data);

            Version version = 0;
            PODS_SAFE_CALL(getVersion<T>(version));

            Member member;
            PODS_SAFE_CALL(getMember(details::PODS_DATA_STR, member));

            if (!member->value.IsArray())
            {
                return Error::CorruptedArchive;
            }

            const Array& array = member->value.GetArray();

            Size size = 0;
            PODS_SAFE_CALL(readSize(array, size));

            Iterator it;
            PODS_SAFE_CALL(init(size, it));

            PODS_SAFE_CALL(readRangeImpl<T>(array, it, version));

            return Error::NoError;
        }

        template <class T, class Iterator, class InitFunction,
            typename std::enable_if<!details::IsPodsSerializable<T>::value, int>::type = 0>
        Error readRange(Value& data, InitFunction init)
        {
            if (!data.IsArray())
            {
                return Error::CorruptedArchive;
            }

            const Array& array = data.GetArray();

            Size size = 0;
            PODS_SAFE_CALL(readSize(array, size));

            Iterator it;
            PODS_SAFE_CALL(init(size, it));

            PODS_SAFE_CALL(readRangeImpl<T>(array, it));

            return Error::NoError;
        }

        template <class T, class Iterator>
        Error readRangeImpl(const Array& array, Iterator begin, Version version)
        {
            for (Size i = 0, size = array.Size(); i < size; ++i, ++begin)
            {
                auto sentry = goDeeperAndBackOnExit(array[i]);
                PODS_SAFE_CALL(deserializeWithoutVersion(*begin, version));
            }

            return Error::NoError;
        }

        template <class T, class Iterator>
        Error readRangeImpl(const Array& array, Iterator begin)
        {
            for (Size i = 0, size = array.Size(); i < size; ++i, ++begin)
            {
                PODS_SAFE_CALL(read(array[i], *begin));
            }

            return Error::NoError;
        }

        template <class T>
        Error getVersion(Version& version)
        {
            PODS_SAFE_CALL(readValue(details::PODS_VERSION_STR, version));
            return version > T::version()
                ? Error::ArchiveVersionMismatch
                : Error::NoError;
        }

    private:
        class AutoNodeLevel final
        {
        public:
            explicit AutoNodeLevel(std::stack<Value*>& nodes, Value* current)
                : nodes_(nodes)
            {
                nodes_.push(current);
            }

            ~AutoNodeLevel()
            {
                nodes_.pop();
            }

        private:
            std::stack<Value*>& nodes_;
        };

        AutoNodeLevel goDeeperAndBackOnExit(Value& value)
        {
            return AutoNodeLevel(nodes_, &value);
        }

        rapidjson::Document document_;
        std::stack<Value*> nodes_;
    };
}
