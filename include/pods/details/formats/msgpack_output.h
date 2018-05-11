#pragma once

#include <cassert>

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
        class MsgPackOutput final
        {
        public:
            explicit MsgPackOutput(Storage& storage) noexcept
                : storage_(storage)
            {
            }

            MsgPackOutput(const MsgPackOutput<Storage>&) = delete;
            MsgPackOutput& operator=(const MsgPackOutput<Storage>&) = delete;

            Error startSerialization() noexcept
            {
                return Error::NoError;
            }

            Error endSerialization()
            {
                storage_.flush();
                return Error::NoError;
            }

            Error saveName(const char*) noexcept
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

            Error startArray(Size size)
            {
                if (size <= msgpack::ArrayMax)
                {
                    const msgpack::Tag tag = static_cast<msgpack::Tag>(size) | msgpack::ArrayMask;
                    return storage_.put(tag);
                }
                else if (size <= msgpack::Max16U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Array16));
                    return storage_.put(static_cast<uint16_t>(size));
                }
                else
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Array32));
                    return storage_.put(static_cast<uint32_t>(size));
                }
            }

            Error endArray() noexcept
            {
                return Error::NoError;
            }

            Error startMap(Size size)
            {
                if (size <= msgpack::MapMax)
                {
                    const msgpack::Tag tag = static_cast<msgpack::Tag>(size) | msgpack::MapMask;
                    return storage_.put(tag);
                }
                else if (size <= msgpack::Max16U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Map16));
                    return storage_.put(static_cast<uint16_t>(size));
                }
                else
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Map32));
                    return storage_.put(static_cast<uint32_t>(size));
                }
            }

            Error endMap() noexcept
            {
                return Error::NoError;
            }

            Error save(int8_t value)
            {
                if (value < msgpack::Min5)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Int8));
                }
                return storage_.put(value);
            }

            Error save(uint8_t value)
            {
                if (value > msgpack::Max7U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::UInt8));
                }
                return storage_.put(value);
            }

            Error save(int16_t value)
            {
                if (value < msgpack::Min8 || value > msgpack::Max8)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Int16));
                    return storage_.put(toBigEndian(value));
                }
                return save(static_cast<int8_t>(value));
            }

            Error save(uint16_t value)
            {
                if (value > msgpack::Max8U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::UInt16));
                    return storage_.put(toBigEndian(value));
                }
                return save(static_cast<uint8_t>(value));
            }

            Error save(int32_t value)
            {
                if (value < msgpack::Min16 || value > msgpack::Max16)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Int32));
                    return storage_.put(toBigEndian(value));
                }
                return save(static_cast<int16_t>(value));
            }

            Error save(uint32_t value)
            {
                if (value > msgpack::Max16U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::UInt32));
                    return storage_.put(toBigEndian(value));
                }
                return save(static_cast<uint16_t>(value));
            }

            Error save(int64_t value)
            {
                if (value < msgpack::Min32 || value > msgpack::Max32)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Int64));
                    return storage_.put(toBigEndian(value));
                }
                return save(static_cast<int32_t>(value));
            }

            Error save(uint64_t value)
            {
                if (value > msgpack::Max32U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::UInt64));
                    return storage_.put(toBigEndian(value));
                }
                return save(static_cast<uint32_t>(value));
            }

            Error save(float value)
            {
                return saveFloat<uint32_t>(msgpack::Float, value);
            }

            Error save(double value)
            {
                return saveFloat<uint64_t>(msgpack::Double, value);
            }

            Error save(bool value)
            {
                return value
                    ? storage_.put(msgpack::True)
                    : storage_.put(msgpack::False);
            }

            Error save(const std::string& value)
            {
                const auto size = value.size();
                if (size <= msgpack::Max8U)
                {
                    if (size <= msgpack::StrMax)
                    {
                        const msgpack::Tag tag = static_cast<msgpack::Tag>(size) | msgpack::StrMask;
                        PODS_SAFE_CALL(storage_.put(tag));
                    }
                    else
                    {
                        PODS_SAFE_CALL(storage_.put(msgpack::Str8));
                        PODS_SAFE_CALL(storage_.put(static_cast<uint8_t>(size)));
                    }
                }
                else if (size <= msgpack::Max16U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Str16));
                    PODS_SAFE_CALL(storage_.put(static_cast<uint16_t>(size)));
                }
                else
                {
                    assert(size <= msgpack::Max32U);
                    PODS_SAFE_CALL(storage_.put(msgpack::Str32));
                    PODS_SAFE_CALL(storage_.put(static_cast<uint32_t>(size)));
                }
                return storage_.put(value.c_str(), size);
            }

            template <class T>
            Error saveBlob(const T* data, Size size)
            {
                if (size <= msgpack::Max8U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Bin8));
                    PODS_SAFE_CALL(storage_.put(static_cast<uint8_t>(size)));
                }
                else if (size <= msgpack::Max16U)
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Bin16));
                    PODS_SAFE_CALL(storage_.put(static_cast<uint16_t>(size)));
                }
                else
                {
                    PODS_SAFE_CALL(storage_.put(msgpack::Bin32));
                    PODS_SAFE_CALL(storage_.put(size));
                }
                return storage_.put(data, size);
            }

        private:
            template <class BytesT, class T>
            Error saveFloat(msgpack::Tag tag, T& value)
            {
                PODS_SAFE_CALL(storage_.put(tag));
                union
                {
                    T value;
                    BytesT bytes;
                } converter;
                converter.value = value;
                return storage_.put(toBigEndian(converter.bytes));
            }

        private:
            Storage& storage_;
        };
    }
}
