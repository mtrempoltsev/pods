#pragma once

#include <array>
#include <deque>
#include <iterator>
#include <list>
#include <forward_list>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "details/binary.h"
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
            const auto result = serialize(std::forward<T&&>(data));
            storage_.flush();
            return result;
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
        Error process(const char*, const T& value, ArgsT&&... args)
        {
            const auto error = doProcess(value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        template <class T, size_t ArraySize>
        Error process(const char*, const T (&value)[ArraySize])
        {
            return doProcess(value);
        }

        template <class T, size_t ArraySize, class... ArgsT>
        Error process(const char*, const T(&value)[ArraySize], ArgsT&... args)
        {
            const auto error = doProcess(value);
            return error == Error::NoError
                ? process(args...)
                : error;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, int>::type = 0>
        Error process(const char*, T value)
        {
            return doProcess(value);
        }

        template <class T, class... ArgsT, typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, int>::type = 0>
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

        Error doProcess(const details::BinaryArray& value)
        {
            return saveRange<const details::BinaryArray::ValueType>(value.data(), value.size());
        }

        template <class T>
        Error doProcess(const details::BinaryVector<T>& value)
        {
            return saveRange<const details::BinaryVector<T>::ValueType>(value.data(), value.size());
        }

        template <class T, size_t ArraySize>
        Error doProcess(const T (&value)[ArraySize])
        {
            return saveRange<const T>(value, ArraySize);
        }

        template <class T, size_t ArraySize>
        Error doProcess(const std::array<T, ArraySize>& value)
        {
            return saveRange<const T>(value.data(), ArraySize);
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
            return saveRange<const T>(value.data(), value.size());
        }

        template <class T>
        Error doProcess(const std::deque<T>& value)
        {
            return saveRange<const T>(value.cbegin(), value.size());
        }

        template <class T>
        Error doProcess(const std::list<T>& value)
        {
            return saveRange<const T>(value.cbegin(), value.size());
        }

        template <class T>
        Error doProcess(const std::forward_list<T>& value)
        {
            size_t size = 0;
            std::for_each(value.begin(), value.end(), [&](const T&) { ++size; });
            return saveRange<const T>(value.cbegin(), size);
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
            PODS_SAFE_CALL(checkSize(size));
            return storage_.put(static_cast<Size>(size));
        }

        template <class T, class Iterator, typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error saveRange(Iterator begin, size_t size)
        {
            PODS_SAFE_CALL(saveSize(size));
            PODS_SAFE_CALL(saveVersion<T>());

            for (size_t i = 0; i < size; ++i)
            {
                PODS_SAFE_CALL(serializeWithoutVersion(*begin++));
            }

            return Error::NoError;
        }

        template <class T, class Iterator, typename std::enable_if<!details::IsPodsSerializable<T>::value, int>::type = 0>
        Error saveRange(Iterator begin, size_t size)
        {
            PODS_SAFE_CALL(saveSize(size));

            for (size_t i = 0; i < size; ++i)
            {
                PODS_SAFE_CALL(doProcess(*begin++));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error saveRange(const T* begin, size_t size)
        {
            PODS_SAFE_CALL(saveSize(size));
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
            PODS_SAFE_CALL(getVersion<T>(version));
            return deserialize(data, version);
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
        Error doProcess(T (&value)[ArraySize])
        {
            Size size = 0;
            PODS_SAFE_CALL(loadSize(size));
            return size == ArraySize
                ? loadRange<T>(value, ArraySize)
                : Error::CorruptedArchive;
        }

        template <class T, size_t ArraySize>
        Error doProcess(std::array<T, ArraySize>& value)
        {
            Size size = 0;
            PODS_SAFE_CALL(loadSize(size));
            return size == ArraySize
                ? loadRange<T>(value.data(), ArraySize)
                : Error::CorruptedArchive;
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
            return loadRange<T>(value.data(), size);
        }

        template <class T>
        Error doProcess(std::deque<T>& value)
        {
            return loadContainer(value);
        }

        template <class T>
        Error doProcess(std::list<T>& value)
        {
            return loadContainer(value);
        }

        template <class T>
        Error doProcess(std::forward_list<T>& value)
        {
            return loadContainer(value);
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

        Error doProcess(details::BinaryArray& value)
        {
            Size size = 0;
            PODS_SAFE_CALL(loadSize(size));
            if (size != value.size())
            {
                return Error::CorruptedArchive;
            }
            PODS_SAFE_CALL(value.allocate(size));
            return loadRange<details::BinaryArray::ValueType>(value.data(), size);
        }

        template <class T>
        Error doProcess(details::BinaryVector<T>& value)
        {
            Size size = 0;
            PODS_SAFE_CALL(loadSize(size));
            PODS_SAFE_CALL(value.allocate(size));
            return loadRange<details::BinaryVector<T>::ValueType>(value.data(), size);
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

        template <class Container>
        Error loadContainer(Container& value)
        {
            Size size = 0;
            PODS_SAFE_CALL(loadSize(size));
            value.resize(size);
            return loadRange<Container::value_type>(value.begin(), size);
        }

        template <class T, class Iterator, typename std::enable_if<details::IsPodsSerializable<T>::value, int>::type = 0>
        Error loadRange(Iterator begin, Size size)
        {
            Version version = 0;
            PODS_SAFE_CALL(getVersion<T>(version));

            for (size_t i = 0; i < size; ++i)
            {
                PODS_SAFE_CALL(deserialize(*begin++, version));
            }

            return Error::NoError;
        }

        template <class T, class Iterator, typename std::enable_if<!details::IsPodsSerializable<T>::value, int>::type = 0>
        Error loadRange(Iterator begin, Size size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                PODS_SAFE_CALL(doProcess(*begin++));
            }

            return Error::NoError;
        }

        template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
        Error loadRange(T* begin, Size size)
        {
            return storage_.get(begin, size);
        }

        template <class T>
        Error getVersion(Version& version)
        {
            PODS_SAFE_CALL(storage_.get(version));
            return version > T::version()
                ? Error::ArchiveVersionMismatch
                : Error::NoError;
        }

    private:
        Storage& storage_;
    };
}
