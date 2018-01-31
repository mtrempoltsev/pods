#pragma once

#include <array>
#include <map>
#include <type_traits>
#include <vector>

#include "details/utils.h"

#include "errors.h"
#include "types.h"

namespace pods
{
    template <class Storage>
    class BinarySerializer final
    {
    public:
        explicit BinarySerializer(Storage& storage) noexcept
            : storage_(storage)
        {
        }

        BinarySerializer(const BinarySerializer<Storage>&) = delete;
        BinarySerializer& operator=(const BinarySerializer<Storage>&) = delete;

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
            PODS_SAFE_CALL(saveVersion<T>());
            return serializeWithoutVersion(value);
        }

        template <class T>
        Error serializeWithoutVersion(const T& value)
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

        template <class T, size_t ArraySize>
        Error doProcess(const std::array<T, ArraySize>& value)
        {
            return saveRange(value.data(), ArraySize);
        }

        template <class K, class V>
        Error doProcess(const std::map<K, V>& value)
        {
            PODS_SAFE_CALL(saveSize(value.size()));

            for (const auto& pair : value)
            {
                PODS_SAFE_CALL(doProcess(pair.first));
                PODS_SAFE_CALL(doProcess(pair.second));
            }

            return Error::NoError;
        }

        template <class T>
        Error doProcess(const std::vector<T>& value)
        {
            PODS_SAFE_CALL(saveSize(value.size()));
            return saveRange(value.data(), value.size());
        }

        Error doProcess(const std::string& value)
        {
            PODS_SAFE_CALL(saveSize(value.size()));
            return storage_.put(value.c_str(), value.size());
        }

        template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        Error doProcess(const T& value)
        {
            return serialize(value);
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

        template <class T, typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error saveRange(T* begin, Size size)
        {
            PODS_SAFE_CALL(saveVersion<T>());

            for (auto end = begin + size; begin != end; ++begin)
            {
                PODS_SAFE_CALL(serializeWithoutVersion(*begin));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_class<T>::value && !details::IsPodsSerializable<T>::value, int>::type = 0>
        Error saveRange(T* begin, Size size)
        {
            for (auto end = begin + size; begin != end; ++begin)
            {
                PODS_SAFE_CALL(doProcess(*begin));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error saveRange(T* begin, Size size)
        {
            return storage_.put(begin, size);
        }

        template <class T>
        Error saveVersion()
        {
            static_assert(details::IsPodsSerializable<T>::value,
                "you must define the methods version() and serialize() for each serializable class or struct");
            return storage_.put(std::decay_t<T>::version());
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

        BinaryDeserializer(const BinaryDeserializer<Storage>&) = delete;
        BinaryDeserializer& operator=(const BinaryDeserializer<Storage>&) = delete;

        template <class T>
        Error load(T& data)
        {
            Version version = 0;
            PODS_SAFE_CALL(storage_.get(version));
            return deserialize(data, version);
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
        Error deserialize(T& value, Version version)
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
            PODS_SAFE_CALL(storage_.get(n));
            value = (n == True);
            return Error::NoError;
        }

        template <class T, size_t ArraySize>
        Error doProcess(std::array<T, ArraySize>& value)
        {
            return loadRange(value.data(), ArraySize);
        }

        template <class K, class V>
        Error doProcess(std::map<K, V>& value)
        {
            Size size = 0;
            PODS_SAFE_CALL(loadSize(size));

            value.clear();

            auto hint = value.begin();
            for (Size i = 0; i < size; ++i)
            {
                K key = K();
                PODS_SAFE_CALL(doProcess(key));

                V val = V();
                PODS_SAFE_CALL(doProcess(val));

                hint = value.emplace_hint(hint, std::move(key), std::move(val));
            }

            return Error::NoError;
        }

        template <class T>
        Error doProcess(std::vector<T>& value)
        {
            Size size = 0;
            PODS_SAFE_CALL(loadSize(size));
            value.resize(size);
            return loadRange(value.data(), size);
        }

        Error doProcess(std::string& value)
        {
            Size size;
            PODS_SAFE_CALL(loadSize(size));
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
            PODS_SAFE_CALL(storage_.get(n));
            value = static_cast<T>(n);
            return Error::NoError;
        }

        Error loadSize(Size& size)
        {
            return storage_.get(size);
        }

        template <class T, typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error loadRange(T* begin, Size size)
        {
            Version version = 0;
            PODS_SAFE_CALL(storage_.get(version));

            for (auto end = begin + size; begin != end; ++begin)
            {
                PODS_SAFE_CALL(deserialize(*begin, version));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_class<T>::value && !details::IsPodsSerializable<T>::value, int>::type = 0>
        Error loadRange(T* begin, Size size)
        {
            for (auto end = begin + size; begin != end; ++begin)
            {
                PODS_SAFE_CALL(doProcess(*begin));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error loadRange(T* begin, Size size)
        {
            return storage_.get(begin, size);
        }

    private:
        Storage& storage_;
    };
}
