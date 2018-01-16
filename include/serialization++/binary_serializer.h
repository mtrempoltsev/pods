#pragma once

#include <type_traits>

#include "errors.h"
#include "name_value.h"

#ifdef SPP_SAFE_CALL
#error Rename the macro
#endif
#define SPP_SAFE_CALL(foo)              \
    {                                   \
        const auto error = (foo);       \
        if (error != Error::NoError)    \
        {                               \
            return error;               \
        }                               \
    }

namespace spp
{
    template <class Storage>
    class BinarySerializer final
    {
    public:
        explicit BinarySerializer(Storage& storage) noexcept
            : storage_(storage)
        {
        }

        BinarySerializer(const BinarySerializer&) = delete;
        BinarySerializer& operator=(const BinarySerializer&) = delete;

        template <class T>
        Error save(const T& data)
        {
            storage_.put(data.actualDataVersion());
            return data.serialize(*this);
        }

        Error operator()() noexcept
        {
            return Error::NoError;
        }

        template <class... T>
        Error operator()(T... args)
        {
            return process(args...);
        }

    private:
        template <class T, class... ArgsT, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error process(const char*, const T& value)
        {
            return doProcess(value);
        }

        template <class T, class... ArgsT, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error process(const char*, const T& value, ArgsT... args)
        {
            const auto error = doProcess(value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        template <class T, class... ArgsT, typename std::enable_if<std::is_scalar<T>::value, int>::type = 0>
        Error process(const char*, T value)
        {
            return doProcess(value);
        }

        template <class T, class... ArgsT, typename std::enable_if<std::is_scalar<T>::value, int>::type = 0>
        Error process(const char*, T value, ArgsT... args)
        {
            const auto error = doProcess(value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        Error doProcess(bool value)
        {
            const uint8_t n = value ? 1 : 0;
            return storage_.put(n);
        }

        template <class T, size_t Size>
        Error doProcess(const std::array<T, Size>& value)
        {
            return saveRange(value.data(), Size);
        }

        template <class K, class V>
        Error doProcess(const std::map<K, V>& value)
        {
            SPP_SAFE_CALL(saveSize(value.size()))

            for (const auto& pair : value)
            {
                SPP_SAFE_CALL(doProcess(pair.first))
                SPP_SAFE_CALL(doProcess(pair.second))
            }

            return Error::NoError;
        }

        template <class T>
        Error doProcess(const std::vector<T>& value)
        {
            return saveRange(value.data(), value.size());
        }

        Error doProcess(const std::string& value)
        {
            SPP_SAFE_CALL(saveSize(value.size()))
            return storage_.put(value.c_str(), value.size());
        }

        template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error doProcess(const T& value)
        {
            return value.serialize(*this);
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error doProcess(T value)
        {
            return storage_.put(value);
        }

        template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        Error doProcess(T value)
        {
            return doProcess(std::underlying_type_t<T>(value));
        }

        Error saveSize(size_t size)
        {
            if (std::numeric_limits<Size>::max() < size)
            {
                return Error::SizeToLarge;
            }

            return storage_.put(static_cast<Size>(size));
        }

        template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error saveRange(T* begin, size_t size)
        {
            SPP_SAFE_CALL(saveSize(size))

            for (auto end = begin + size; begin != end; ++begin)
            {
                SPP_SAFE_CALL(doProcess(*begin))
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error saveRange(T* begin, size_t size)
        {
            SPP_SAFE_CALL(saveSize(size))
            return storage_.put(begin, size);
        }

    private:
        Storage& storage_;
    };

    template <class Storage>
    class BinaryDeserializer final
    {
    public:
        explicit BinaryDeserializer(Storage& storage) noexcept
            : storage_(storage)
        {
        }

        BinaryDeserializer(const BinaryDeserializer&) = delete;
        BinaryDeserializer& operator=(const BinaryDeserializer&) = delete;

        template <class T>
        Error load(T& data)
        {
            Version version = 0;
            storage_.get(version);
            return data.deserialize(*this, version);
        }

        template <class T>
        Error operator()(MandatoryNameValue<T>&& value)
        {
            return Error::NoError;
        }

    private:
        Storage& storage_;
    };
}

#undef SPP_SAFE_CALL
