#pragma once

#include <rapidjson/reader.h>

#include "../base64.h"
#include "../names.h"
#include "../rapidjson_wrappers.h"
#include "../utils.h"

#include "../../errors.h"
#include "../../types.h"

namespace pods
{
    namespace details
    {
        struct StartObjectHandler final
        {
            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }
            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }
            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }

            bool StartObject() noexcept
            {
                return true;
            }

            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept { return false; }
            bool StartArray() noexcept { return false; }
            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }
        };

        struct EndObjectHandler final
        {
            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }
            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }
            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool StartObject() noexcept { return false; }
            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }

            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept
            {
                return true;
            }

            bool StartArray() noexcept { return false; }
            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }
        };

        struct StartArrayHandler final
        {
            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }
            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }
            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool StartObject() noexcept { return false; }
            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept { return false; }

            bool StartArray() noexcept
            {
                return true;
            }

            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }
        };

        struct EndArrayHandler final
        {
            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }
            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }
            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool StartObject() noexcept { return false; }
            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept { return false; }
            bool StartArray() noexcept { return false; }

            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept
            {
                return true;
            }
        };

        struct KeyHandler final
        {
            KeyHandler(const char* key, std::string& nextKey, bool& isEndOfObject) noexcept
                : key_(key)
                , nextKey_(nextKey)
                , isEndOfObject_(isEndOfObject)
            {
            }

            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }
            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }
            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool StartObject() noexcept { return false; }

            bool Key(const char* str, rapidjson::SizeType length, bool /*copy*/)
            {
                if (strncmp(key_, str, length) != 0)
                {
                    nextKey_.assign(str, length);
                }
                return true;
            }

            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept
            {
                isEndOfObject_ = true;
                return true;
            }

            bool StartArray() noexcept { return false; }
            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }

        private:
            const char* key_;
            std::string& nextKey_;
            bool& isEndOfObject_;
        };

        struct BoolHandler final
        {
            explicit BoolHandler(bool& value) noexcept
                : value_(value)
            {
            }

            bool Null() noexcept { return false; }

            bool Bool(bool value) noexcept
            {
                value_ = value;
                return true;
            }

            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }
            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool StartObject() noexcept { return false; }
            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept { return false; }
            bool StartArray() noexcept { return false; }
            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }

        private:
            bool& value_;
        };

        template <class T>
        struct IntHandler final
        {
            explicit IntHandler(T& value) noexcept
                : value_(value)
            {
            }

            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }

            bool Int(int value) noexcept
            {
                return Int64(value);
            }

            bool Uint(unsigned value) noexcept
            {
                return Uint64(value);
            }

            bool Int64(int64_t value) noexcept
            {
                const int64_t min = std::numeric_limits<T>::min() < 0
                    ? std::numeric_limits<T>::min()
                    : 0;

                if (value < min)
                {
                    return false;
                }

                if (value > 0 && static_cast<uint64_t>(value) > static_cast<uint64_t>(std::numeric_limits<T>::max()))
                {
                    return false;
                }

                value_ = static_cast<T>(value);
                return true;
            }

            bool Uint64(uint64_t value) noexcept
            {
                if (value > static_cast<uint64_t>(std::numeric_limits<T>::max()))
                {
                    return false;
                }

                value_ = static_cast<T>(value);
                return true;
            }

            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool StartObject() noexcept { return false; }
            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept { return false; }
            bool StartArray() noexcept { return false; }
            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }

        private:
            T& value_;
        };

        template <class T>
        struct FloatingHandler final
        {
            explicit FloatingHandler(T& value) noexcept
                : value_(value)
            {
            }

            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }
            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }

            bool Double(double value) noexcept
            {
                if (value < std::numeric_limits<T>::min() || value > std::numeric_limits<T>::max())
                {
                    return false;
                }
                value_ = static_cast<T>(value);
                return true;
            }

            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool String(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool StartObject() noexcept { return false; }
            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept { return false; }
            bool StartArray() noexcept { return false; }
            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }

        private:
            T& value_;
        };

        struct StringHandler final
        {
            explicit StringHandler(std::string& value) noexcept
                : value_(value)
            {
            }

            bool Null() noexcept { return false; }
            bool Bool(bool /*value*/) noexcept { return false; }
            bool Int(int /*value*/) noexcept { return false; }
            bool Uint(unsigned /*value*/) noexcept { return false; }
            bool Int64(int64_t /*value*/) noexcept { return false; }
            bool Uint64(uint64_t /*value*/) noexcept { return false; }
            bool Double(double /*value*/) noexcept { return false; }
            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }

            bool String(const char* str, rapidjson::SizeType length, bool /*copy*/)
            {
                value_.assign(str, length);
                return true;
            }

            bool StartObject() noexcept { return false; }
            bool Key(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }
            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept { return false; }
            bool StartArray() noexcept { return false; }
            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept { return false; }

        private:
            std::string& value_;
        };

        template <class Storage>
        class JsonInput final
        {
        public:
            explicit JsonInput(Storage& storage)
                : stream_(storage)
            {
                reader_.IterativeParseInit();
            }

            JsonInput(const JsonInput<Storage>&) = delete;
            JsonInput& operator=(const JsonInput<Storage>&) = delete;

            Error startDeserialization()
            {
                isEndOfObject_ = false;
                return startObject();
            }

            Error endDeserialization()
            {
                return isEndOfObject_
                    ? Error::NoError
                    : endObject();
            }

            Error checkName(const char* name)
            {
                if (nextName_.empty())
                {
                    KeyHandler handler(getName(name), nextName_, isEndOfObject_);
                    if (reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler))
                    {
                        if (!isEndOfObject_ && nextName_.empty())
                        {
                            return Error::NoError;
                        }

                        return isOptional(name)
                            ? Error::OptionalFieldMissed
                            : Error::MandatoryFieldMissed;
                    }

                    return Error::CorruptedArchive;
                }

                if (nextName_ == getName(name))
                {
                    nextName_.clear();
                    return Error::NoError;
                }

                return isOptional(name)
                    ? Error::OptionalFieldMissed
                    : Error::MandatoryFieldMissed;
            }

            Error startObject()
            {
                StartObjectHandler handler;
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            Error endObject()
            {
                EndObjectHandler handler;
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            Error startArray(Size& size)
            {
                PODS_SAFE_CALL(checkName(PODS_SIZE));
                PODS_SAFE_CALL(load(size));
                PODS_SAFE_CALL(checkName(PODS_DATA));
                StartArrayHandler handler;
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            Error endArray()
            {
                EndArrayHandler handler;
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            Error startMap(Size& size)
            {
                return startArray(size);
            }

            Error endMap()
            {
                return endArray();
            }

            template <class T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
            Error load(T& value)
            {
                FloatingHandler<T> handler(value);
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            template <class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            Error load(T& value)
            {
                IntHandler<T> handler(value);
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            Error load(bool& value)
            {
                BoolHandler handler(value);
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            Error load(std::string& value)
            {
                StringHandler handler(value);
                return reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler)
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            Error loadBlob(char* data, size_t size)
            {
                std::string encoded;
                PODS_SAFE_CALL(load(encoded));

                const auto decodedSize = details::getBase64DecodedSize(encoded.c_str(), encoded.size());
                if (decodedSize != size)
                {
                    return Error::CorruptedArchive;
                }

                details::base64Decode(encoded.c_str(), encoded.size(), data);
                return Error::NoError;
            }

            template <class Allocator>
            Error loadBlob(const Allocator& allocator)
            {
                std::string encoded;
                PODS_SAFE_CALL(load(encoded));

                const auto decodedSize = details::getBase64DecodedSize(encoded.c_str(), encoded.size());
                PODS_SAFE_CALL(checkSize(decodedSize));
                char* data = nullptr;
                PODS_SAFE_CALL(allocator(data, static_cast<Size>(decodedSize)));

                details::base64Decode(encoded.c_str(), encoded.size(), data);
                return Error::NoError;
            }

        private:
            InputRapidJsonStreamWrapper<Storage> stream_;
            rapidjson::Reader reader_;

            std::string nextName_;
            bool isEndOfObject_;
        };
    }
}
