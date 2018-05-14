#pragma once

#include "../endianness.h"
#include "../utils.h"

#include "../../errors.h"
#include "../../types.h"

#include "msgpack_aux.h"

namespace pods
{
    namespace details
    {
        template <class Storage>
        class MsgPackInput final
        {
        public:
            explicit MsgPackInput(Storage& storage) noexcept
                : storage_(storage)
            {
            }

            MsgPackInput(const MsgPackInput<Storage>&) = delete;
            MsgPackInput& operator=(const MsgPackInput<Storage>&) = delete;

            Error startDeserialization() noexcept
            {
                return Error::NoError;
            }

            Error endDeserialization() noexcept
            {
                return Error::NoError;
            }

            Error checkName(const char*) noexcept
            {
                return Error::NoError;
            }

            Error startObject() noexcept
            {
                return Error::NoError;
            }

            Error endObject() noexcept
            {
                return Error::NoError;
            }

            Error startArray(Size& size)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::Array16:
                {
                    uint16_t n = 0;
                    PODS_SAFE_CALL(storage_.get(n));
                    size = n;
                    return Error::NoError;
                }
                case msgpack::Array32:
                    PODS_SAFE_CALL(storage_.get(size));
                    return Error::NoError;
                }

                if ((tag & ~msgpack::ArrayValue) == msgpack::ArrayMask)
                {
                    size = (tag & msgpack::ArrayValue);
                    return Error::NoError;
                }

                return Error::CorruptedArchive;
            }

            Error endArray() noexcept
            {
                return Error::NoError;
            }

            Error startMap(Size& size)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::Map16:
                {
                    uint16_t n = 0;
                    PODS_SAFE_CALL(storage_.get(n));
                    size = n;
                    return Error::NoError;
                }
                case msgpack::Map32:
                    PODS_SAFE_CALL(storage_.get(size));
                    return Error::NoError;
                }

                if ((tag & ~msgpack::MapValue) == msgpack::MapMask)
                {
                    size = (tag & msgpack::MapValue);
                    return Error::NoError;
                }

                return Error::CorruptedArchive;
            }

            Error endMap() noexcept
            {
                return Error::NoError;
            }

            Error load(int8_t& value)
            {
                PODS_SAFE_CALL(storage_.get(value));

                if (value < msgpack::Min5)
                {
                    if (static_cast<msgpack::Tag>(value) != msgpack::Int8)
                    {
                        return Error::CorruptedArchive;
                    }
                    return storage_.get(value);
                }

                return Error::NoError;
            }

            Error load(uint8_t& value)
            {
                PODS_SAFE_CALL(storage_.get(value));

                if (value > msgpack::Max7U)
                {
                    if (value == msgpack::UInt8)
                    {
                        return storage_.get(value);
                    }
                    return Error::CorruptedArchive;
                }

                return Error::NoError;
            }

            Error load(int16_t& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::Int8:
                    return doLoad<int8_t>(value);
                case msgpack::Int16:
                    return doLoad<int16_t>(value);
                }

                return tagToInt8(tag, value);
            }

            Error load(uint16_t& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::UInt8:
                    return doLoad<uint8_t>(value);
                case msgpack::UInt16:
                    return doLoad<uint16_t>(value);
                }

                return tagToUInt8(tag, value);
            }

            Error load(int32_t& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::Int8:
                    return doLoad<int8_t>(value);
                case msgpack::Int16:
                    return doLoad<int16_t>(value);
                case msgpack::Int32:
                    return doLoad<int32_t>(value);
                }

                return tagToInt8(tag, value);
            }

            Error load(uint32_t& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::UInt8:
                    return doLoad<uint8_t>(value);
                case msgpack::UInt16:
                    return doLoad<uint16_t>(value);
                case msgpack::UInt32:
                    return doLoad<uint32_t>(value);
                }

                return tagToUInt8(tag, value);
            }

            Error load(int64_t& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::Int8:
                    return doLoad<int8_t>(value);
                case msgpack::Int16:
                    return doLoad<int16_t>(value);
                case msgpack::Int32:
                    return doLoad<int32_t>(value);
                case msgpack::Int64:
                    return doLoad<int64_t>(value);
                }

                return tagToInt8(tag, value);
            }

            Error load(uint64_t& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::UInt8:
                    return doLoad<uint8_t>(value);
                case msgpack::UInt16:
                    return doLoad<uint16_t>(value);
                case msgpack::UInt32:
                    return doLoad<uint32_t>(value);
                case msgpack::UInt64:
                    return doLoad<uint64_t>(value);
                }

                return tagToUInt8(tag, value);
            }

            Error load(float& value)
            {
                return loadFloat<uint32_t>(msgpack::Float, value);
            }

            Error load(double& value)
            {
                return loadFloat<uint64_t>(msgpack::Double, value);
            }

            Error load(bool& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));
                switch (tag)
                {
                case msgpack::False:
                    value = false;
                    return Error::NoError;
                case msgpack::True:
                    value = true;
                    return Error::NoError;
                }
                return Error::CorruptedArchive;
            }

            Error load(std::string& value)
            {
                Size size = 0;
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));
                switch (tag)
                {
                case msgpack::Str8:
                {
                    uint8_t n = 0;
                    PODS_SAFE_CALL(storage_.get(n));
                    size = n;
                    break;
                }
                case msgpack::Str16:
                {
                    uint16_t n = 0;
                    PODS_SAFE_CALL(storage_.get(n));
                    size = n;
                    break;
                }
                case msgpack::Str32:
                    PODS_SAFE_CALL(storage_.get(size));
                    break;
                default:
                    if ((tag & ~msgpack::StrValue) == msgpack::StrMask)
                    {
                        size = (tag & msgpack::StrValue);
                    }
                    else
                    {
                        return Error::CorruptedArchive;
                    }
                }
                value.resize(size);
                return storage_.get(const_cast<char*>(value.data()), size);
            }

            Error loadBlob(char* data, size_t size)
            {
                Size actualSize = 0;
                PODS_SAFE_CALL(loadBinarySize(actualSize));
                return size == actualSize
                    ? storage_.get(data, actualSize)
                    : Error::CorruptedArchive;
            }

            template <class Allocator>
            Error loadBlob(const Allocator& allocator)
            {
                Size size = 0;
                PODS_SAFE_CALL(loadBinarySize(size));
                char* data = nullptr;
                PODS_SAFE_CALL(allocator(data, size));
                return storage_.get(data, size);
            }

        private:
            template <class BytesT, class T>
            Error loadFloat(msgpack::Tag expectedTag, T& value)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));
                if (tag == expectedTag)
                {
                    union
                    {
                        T value;
                        BytesT bytes;
                    } converter;
                    PODS_SAFE_CALL(storage_.get(converter.bytes));
                    converter.bytes = fromBigEndian(converter.bytes);
                    value = converter.value;
                    return Error::NoError;
                }
                return Error::CorruptedArchive;
            }

            Error loadBinarySize(Size& size)
            {
                msgpack::Tag tag;
                PODS_SAFE_CALL(storage_.get(tag));

                switch (tag)
                {
                case msgpack::Bin8:
                {
                    uint8_t n = 0;
                    PODS_SAFE_CALL(storage_.get(n));
                    size = n;
                    return Error::NoError;
                }
                case msgpack::Bin16:
                {
                    uint16_t n = 0;
                    PODS_SAFE_CALL(storage_.get(n));
                    size = n;
                    return Error::NoError;
                }
                case msgpack::Bin32:
                    return storage_.get(size);
                }

                return Error::CorruptedArchive;
            }

            template <class T>
            Error tagToInt8(uint8_t tag, T& value) noexcept
            {
                if (tag <= msgpack::Max8U && static_cast<int8_t>(tag) >= msgpack::Min8)
                {
                    value = static_cast<T>(tag);
                    return Error::NoError;
                }
                return Error::CorruptedArchive;
            }

            template <class T>
            Error tagToUInt8(uint8_t tag, T& value) noexcept
            {
                if (tag <= msgpack::Max7U)
                {
                    value = tag;
                    return Error::NoError;
                }
                return Error::CorruptedArchive;
            }

            template <class FromT, class T>
            Error doLoad(T& value)
            {
                FromT n;
                PODS_SAFE_CALL(storage_.get(n));
                value = fromBigEndian(n);
                return Error::NoError;
            }

        private:
            Storage& storage_;
        };
    }
}
