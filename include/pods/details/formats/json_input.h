#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

#include <rapidjson/reader.h>

#include "../base64.h"
#include "../rapidjson_wrappers.h"
#include "../serialization_traits.h"
#include "../utils.h"

#include "../../errors.h"
#include "../../types.h"

namespace pods
{
    namespace details
    {
        struct StartObjectTag final {};
        struct EndObjectTag final {};
        struct StartArrayTag final {};
        struct EndArrayTag final {};
        struct KeyTag final {};
        struct BoolTag final {};
        struct IntTag final {};
        struct FloatTag final {};
        struct StringTag final {};

        struct EmptyData final
        {
            using ResultT = void;
        };

        struct IsEndOfArrayData
        {
            bool isEndOfArray_ = false;
        };

        struct IsEndOfObjectData
        {
            bool isEndOfObject_ = false;
        };

        template <class T>
        struct ValueData
        {
            using ValueT = T;

            explicit ValueData(T& value) noexcept
                : value_(value)
            {
            }

            T& value_;
        };

        template <class T>
        struct ValueAndIsEndOfArrayData final
            : public IsEndOfArrayData
            , public ValueData<T>
        {
            explicit ValueAndIsEndOfArrayData(T& value) noexcept
                : ValueData<T>(value)
            {
            }
        };

        template <class T>
        struct ValueAndIsEndOfObjectData final
            : public IsEndOfObjectData
            , public ValueData<T>
        {
            explicit ValueAndIsEndOfObjectData(T& value) noexcept
                : ValueData<T>(value)
            {
            }
        };

        template <class T>
        struct ValueAndIsEndOfArrayOrObjectData final
            : public IsEndOfArrayData
            , public IsEndOfObjectData
            , public ValueData<T>
        {
            explicit ValueAndIsEndOfArrayOrObjectData(T& value) noexcept
                : ValueData<T>(value)
            {
            }
        };

        template <class Tag, class Data>
        struct Handler final
        {
            using DataT = Data;

            Handler()
                : data_{}
            {
            }

            template <class T>
            Handler(T& value, typename std::enable_if_t<std::is_base_of<ValueData<T>, DataT>::value>* = nullptr)
                : data_{ value }
            {
            }

            bool Null() noexcept { return false; }

            bool Bool(bool value) noexcept
            {
                if constexpr (std::is_same<Tag, BoolTag>::value)
                {
                    data_.value_ = value;
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool Int(int value) noexcept
            {
                if constexpr (std::is_same<Tag, IntTag>::value)
                {
                    return Int64(value);
                }
                else
                {
                    return false;
                }
            }

            bool Uint(unsigned value) noexcept
            {
                if constexpr (std::is_same<Tag, IntTag>::value)
                {
                    return Uint64(value);
                }
                else
                {
                    return false;
                }
            }

            bool Int64(int64_t value) noexcept
            {
                if constexpr (std::is_same<Tag, IntTag>::value)
                {
                    using T = typename DataT::ValueT;

                    if (std::cmp_greater_equal(value, std::numeric_limits<T>::min())
                        && std::cmp_less_equal(value, std::numeric_limits<T>::max()))
                    {
                        data_.value_ = static_cast<T>(value);
                        return true;
                    }

                    return false;
                }
                else
                {
                    return false;
                }
            }

            bool Uint64(uint64_t value) noexcept
            {
                if constexpr (std::is_same<Tag, IntTag>::value)
                {
                    using T = typename DataT::ValueT;

                    if (std::cmp_less_equal(value, std::numeric_limits<T>::max()))
                    {
                        data_.value_ = static_cast<T>(value);
                        return true;
                    }

                    return false;
                }
                else
                {
                    return false;
                }
            }

            bool Double(double value) noexcept
            {
                if constexpr (std::is_same<Tag, FloatTag>::value)
                {
                    using T = typename DataT::ValueT;

                    if (value >= std::numeric_limits<T>::lowest() && value <= std::numeric_limits<T>::max())
                    {
                        data_.value_ = static_cast<T>(value);
                        return true;
                    }

                    return false;
                }
                else
                {
                    return false;
                }
            }

            bool RawNumber(const char* /*str*/, rapidjson::SizeType /*length*/, bool /*copy*/) noexcept { return false; }

            bool String(const char* str, rapidjson::SizeType length, bool /*copy*/) noexcept
            {
                if constexpr (std::is_same<Tag, StringTag>::value)
                {
                    data_.value_.assign(str, length);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool StartObject() noexcept
            {
                if constexpr (std::is_same<Tag, StartObjectTag>::value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool Key(const char* str, rapidjson::SizeType length, bool /*copy*/) noexcept
            {
                if constexpr (std::is_same<Tag, KeyTag>::value
                    || std::is_same<Tag, KeyTag>::value)
                {
                    data_.value_.assign(str, length);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool EndObject(rapidjson::SizeType /*memberCount*/) noexcept
            {
                if constexpr (std::is_same<Tag, KeyTag>::value)
                {
                    data_.isEndOfObject_ = true;
                    return true;
                }
                else if constexpr (std::is_same<Tag, EndObjectTag>::value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool StartArray() noexcept
            {
                if constexpr (std::is_same<Tag, StartArrayTag>::value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool EndArray(rapidjson::SizeType /*elementCount*/) noexcept
            {
                if constexpr (std::is_same<Tag, StartObjectTag>::value
                    || std::is_same<Tag, StartArrayTag>::value
                    || std::is_same<Tag, StringTag>::value
                    || std::is_same<Tag, IntTag>::value
                    || (std::is_same<Tag, KeyTag>::value && std::is_base_of<IsEndOfArrayData, Data>::value)
                    || std::is_same<Tag, BoolTag>::value
                    || std::is_same<Tag, FloatTag>::value)
                {
                    data_.isEndOfArray_ = true;
                    return true;
                }
                else if constexpr (std::is_same<Tag, EndArrayTag>::value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            Data data_;
        };

        template <class Storage>
        class JsonInput final
        {
        public:
            using Traits = JsonTraits;

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
                if (name_.empty())
                {
                    Handler<KeyTag, ValueAndIsEndOfObjectData<std::string>> handler(name_);
                    PODS_SAFE_CALL(parseNext(handler));

                    isEndOfObject_ = handler.data_.isEndOfObject_;

                    if (isEndOfObject_)
                    {
                        return isOptional(name)
                            ? Error::OptionalFieldMissed
                            : Error::MandatoryFieldMissed;
                    }
                }

                if (strcmp(name_.c_str(), getName(name)) == 0)
                {
                    name_.clear();
                    return Error::NoError;
                }

                return isOptional(name)
                    ? Error::OptionalFieldMissed
                    : Error::MandatoryFieldMissed;
            }

            Error startObject()
            {
                Handler<StartObjectTag, IsEndOfArrayData> handler;
                return parseNext(handler);
            }

            Error endObject()
            {
                Handler<EndObjectTag, EmptyData> handler;
                return parseNext(handler);
            }

            Error startArray()
            {
                Handler<StartArrayTag, IsEndOfArrayData> handler;
                return parseNext(handler);
            }

            Error endArray()
            {
                Handler<EndArrayTag, EmptyData> handler;
                return parseNext(handler);
            }

            Error startMap()
            {
                return startObject();
            }

            Error endMap()
            {
                return endObject();
            }

            Error loadKey(std::string& key)
            {
                Handler<KeyTag, ValueAndIsEndOfArrayOrObjectData<std::string>> handler(key);
                return parseNext(handler);
            }

            template <class T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
            Error load(T& value)
            {
                Handler<FloatTag, ValueAndIsEndOfArrayData<T>> handler(value);
                return parseNext(handler);
            }

            template <class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            Error load(T& value)
            {
                Handler<IntTag, ValueAndIsEndOfArrayData<T>> handler(value);
                return parseNext(handler);
            }

            Error load(bool& value)
            {
                Handler<BoolTag, ValueAndIsEndOfArrayData<bool>> handler(value);
                return parseNext(handler);
            }

            Error load(std::string& value)
            {
                Handler<StringTag, ValueAndIsEndOfArrayData<std::string>> handler(value);
                return parseNext(handler);
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
            template <class Handler>
            Error parseNext(Handler handler)
            {
                if (reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(stream_, handler))
                {
                    if constexpr (std::is_base_of<IsEndOfArrayData, Handler::DataT>::value
                        && std::is_base_of<IsEndOfObjectData, Handler::DataT>::value)
                    {
                        if (handler.data_.isEndOfArray_)
                        {
                            return Error::EndOfArray;
                        }
                        else if (handler.data_.isEndOfObject_)
                        {
                            return Error::EndOfObject;
                        }
                        else
                        {
                            return Error::NoError;
                        }
                    }
                    else if constexpr (std::is_base_of<IsEndOfArrayData, Handler::DataT>::value)
                    {
                        return handler.data_.isEndOfArray_
                            ? Error::EndOfArray
                            : Error::NoError;
                    }
                    else if constexpr (std::is_base_of<IsEndOfObjectData, Handler::DataT>::value)
                    {
                        return handler.data_.isEndOfObject_
                            ? Error::EndOfObject
                            : Error::NoError;
                    }
                    else
                    {
                        return Error::NoError;
                    }
                }

                return Error::CorruptedArchive;
            }

        private:
            InputRapidJsonStreamWrapper<Storage> stream_;
            rapidjson::Reader reader_;

            std::string name_;
            bool isEndOfObject_;
        };
    }
}
