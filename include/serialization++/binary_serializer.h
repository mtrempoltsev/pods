#pragma once

#include <array>
#include <map>
#include <type_traits>
#include <vector>

#include "errors.h"
#include "types.h"

#ifdef SPP_SAFE_CALL
#error Rename the macro
#endif
#define SPP_SAFE_CALL(foo)              \
    do                                  \
    {                                   \
        const auto error = (foo);       \
        if (error != Error::NoError)    \
        {                               \
            return error;               \
        }                               \
    } while (false)

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
            storage_.put(T::version());
            return saveData(data);
        }

        Error operator()() noexcept
        {
            return Error::NoError;
        }

        template <class... T>
        Error operator()(T&... args)
        {
            return process(args...);
        }

    private:
        template <class T>
        Error saveData(const T& value)
        {
            return const_cast<T&>(value).serialize(*this, T::version());
        }

        template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error process(const char*, const T& value)
        {
            return doProcess(value);
        }

        template <class T, class... ArgsT, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error process(const char*, const T& value, ArgsT&... args)
        {
            const auto error = doProcess(value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        template <class T, typename std::enable_if<std::is_scalar<T>::value, int>::type = 0>
        Error process(const char*, T value)
        {
            return doProcess(value);
        }

        template <class T, class... ArgsT, typename std::enable_if<std::is_scalar<T>::value, int>::type = 0>
        Error process(const char*, T value, ArgsT&... args)
        {
            const auto error = doProcess(value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        Error doProcess(bool value)
        {
            const Bool n = value ? True : False;
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
            SPP_SAFE_CALL(saveSize(value.size()));

            for (const auto& pair : value)
            {
                SPP_SAFE_CALL(doProcess(pair.first));
                SPP_SAFE_CALL(doProcess(pair.second));
            }

            return Error::NoError;
        }

        template <class T>
        Error doProcess(const std::vector<T>& value)
        {
            SPP_SAFE_CALL(saveSize(value.size()));
            return saveRange(value.data(), value.size());
        }

        Error doProcess(const std::string& value)
        {
            SPP_SAFE_CALL(saveSize(value.size()));
            return storage_.put(value.c_str(), value.size());
        }

        template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error doProcess(const T& value)
        {
            return save(value);
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
            SPP_SAFE_CALL(storage_.put(T::version()));

            for (auto end = begin + size; begin != end; ++begin)
            {
                SPP_SAFE_CALL(saveData(*begin));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error saveRange(T* begin, size_t size)
        {
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
            SPP_SAFE_CALL(storage_.get(version));
            return loadData(data, version);
        }

        Error operator()() noexcept
        {
            return Error::NoError;
        }

        template <class... T>
        Error operator()(T&... args)
        {
            return process(args...);
        }

    private:
        template <class T>
        Error loadData(T& value, Version version)
        {
            return value.serialize(*this, version);
        }

        template <class T>
        Error process(const char*, T& value)
        {
            return doProcess(value);
        }

        template <class T, class... ArgsT>
        Error process(const char*, T& value, ArgsT&... args)
        {
            const auto error = doProcess(value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        Error doProcess(bool& value)
        {
            Bool n = False;
            SPP_SAFE_CALL(storage_.get(n));
            value = (n == True);
            return Error::NoError;
        }

        template <class T, size_t Size>
        Error doProcess(std::array<T, Size>& value)
        {
            return loadRange(value.data(), Size);
        }

        template <class K, class V>
        Error doProcess(std::map<K, V>& value)
        {
            Size size = 0;
            SPP_SAFE_CALL(loadSize(size));

            value.clear();

            auto hint = value.begin();
            for (Size i = 0; i < size; ++i)
            {
                K key = K();
                SPP_SAFE_CALL(doProcess(key));

                V val = V();
                SPP_SAFE_CALL(doProcess(val));

                hint = value.emplace_hint(hint, std::move(key), std::move(val));
            }

            return Error::NoError;
        }

        template <class T>
        Error doProcess(std::vector<T>& value)
        {
            Size size = 0;
            SPP_SAFE_CALL(loadSize(size));
            value.resize(size);
            return loadRange(value.data(), size);
        }

        Error doProcess(std::string& value)
        {
            Size size;
            SPP_SAFE_CALL(loadSize(size));
            value.resize(size);
            return size > 0
                ? storage_.get(&value[0], size)
                : Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error doProcess(T& value)
        {
            return load(value);
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error doProcess(T& value)
        {
            return storage_.get(value);
        }

        template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        Error doProcess(T& value)
        {
            std::underlying_type_t<T> n = 0;
            SPP_SAFE_CALL(storage_.get(n));
            value = static_cast<T>(n);
            return Error::NoError;
        }

        Error loadSize(Size& size)
        {
            return storage_.get(size);
        }

        template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error loadRange(T* begin, size_t size)
        {
            Version version = 0;
            SPP_SAFE_CALL(storage_.get(version));

            for (auto end = begin + size; begin != end; ++begin)
            {
                SPP_SAFE_CALL(loadData(*begin, version));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error loadRange(T* begin, size_t size)
        {
            return storage_.get(begin, size);
        }

    private:
        Storage& storage_;
    };
}

#undef SPP_SAFE_CALL
