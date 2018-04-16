#pragma once

#include <algorithm>
#include <type_traits>

#include <string>

#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>

#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <stack>
#include <queue>

#include "binary_wrappers.h"
#include "names.h"
#include "utils.h"

#include "../errors.h"
#include "../types.h"

namespace pods
{
    namespace details
    {
        template <class Format, class Storage>
        class Serializer final
        {
        public:
            explicit Serializer(Storage& storage) noexcept
                : format_(storage)
            {
            }

            Serializer(const Serializer<Format, Storage>&) = delete;
            Serializer& operator=(const Serializer<Format, Storage>&) = delete;

            template <class T>
            Error save(T&& data)
            {
                return serialize(std::forward<T>(data));
            }

            Error operator()() noexcept
            {
                return Error::NoError;
            }

            template <class... ArgsT>
            Error operator()(ArgsT&&... args)
            {
                return process(std::forward<ArgsT>(args)...);
            }

        private:
            template <class T>
            Error serialize(const T& value)
            {
                static_assert(IsPodsSerializable<T>::value,
                    "You must define the methods version() and serialize() for each serializable struct");

                PODS_SAFE_CALL(format_.startSerialization());
                PODS_SAFE_CALL(saveVersion<T>(PODS_VERSION));
                PODS_SAFE_CALL(const_cast<T&>(value).serialize(*this, T::version()));
                return format_.endSerialization();
            }

            template <class T>
            Error doSerialize(const T& value)
            {
                static_assert(IsPodsSerializable<T>::value,
                    "You must define the methods version() and serialize() for each serializable struct");

                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(saveVersion<T>(PODS_VERSION));
                PODS_SAFE_CALL(const_cast<T&>(value).serialize(*this, T::version()));
                return format_.endObject();
            }

            template <class T>
            Error doSerializeWithoutVersion(const T& value)
            {
                static_assert(IsPodsSerializable<T>::value,
                    "You must define the methods version() and serialize() for each serializable struct");

                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(const_cast<T&>(value).serialize(*this, T::version()));
                return format_.endObject();
            }

            template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error process(const char* name, const T& value)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                return doProcess(value);
            }

            template <class T, class... ArgsT, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error process(const char* name, const T& value, ArgsT&&... args)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                const auto error = doProcess(value);
                return error == Error::NoError
                    ? process(std::forward<ArgsT>(args)...)
                    : error;
            }

            template <class T, size_t ArraySize>
            Error process(const char* name, const T(&value)[ArraySize])
            {
                PODS_SAFE_CALL(format_.saveName(name));
                return doProcess(value);
            }

            template <class T, size_t ArraySize, class... ArgsT>
            Error process(const char* name, const T(&value)[ArraySize], ArgsT&&... args)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                const auto error = doProcess(value);
                return error == Error::NoError
                    ? process(std::forward<ArgsT>(args)...)
                    : error;
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, int>::type = 0>
            Error process(const char* name, T value)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                return doProcess(value);
            }

            template <class T, class... ArgsT, typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, int>::type = 0>
            Error process(const char* name, T value, ArgsT&&... args)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                const auto error = doProcess(value);
                return error == Error::NoError
                    ? process(std::forward<ArgsT>(args)...)
                    : error;
            }

            template <class T, typename std::enable_if<IsPodsSerializable<T>::value, int>::type = 0>
            Error doProcessWithoutVersion(const T& value)
            {
                return doSerializeWithoutVersion(value);
            }

            template <class T, typename std::enable_if<std::is_class<T>::value && !IsPodsSerializable<T>::value, int>::type = 0>
            Error doProcessWithoutVersion(const T& value)
            {
                return doProcess(value);
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, int>::type = 0>
            Error doProcessWithoutVersion(T value)
            {
                return doProcess(value);
            }

            template <class Key, class Val>
            Error doProcessWithoutVersion(const std::pair<Key, Val>& value)
            {
                PODS_SAFE_CALL(format_.startObject());

                PODS_SAFE_CALL(format_.saveName(PODS_KEY));
                PODS_SAFE_CALL(doProcessWithoutVersion(value.first));

                PODS_SAFE_CALL(format_.saveName(PODS_VAL));
                PODS_SAFE_CALL(doProcessWithoutVersion(value.second));

                return format_.endObject();
            }

            template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error doProcess(const T& value)
            {
                return doSerialize(value);
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
            Error doProcess(T value)
            {
                return format_.save(value);
            }

            template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
            Error doProcess(T value)
            {
                return format_.save(std::underlying_type_t<T>(value));
            }

            Error doProcess(const std::string& value)
            {
                PODS_SAFE_CALL(checkSize(value.size()));
                return format_.save(value);
            }

            Error doProcess(const BinaryArray& value)
            {
                const auto size = value.size();
                PODS_SAFE_CALL(checkSize(size));
                return format_.saveBlob(value.data(), static_cast<Size>(size));
            }

            template <class T>
            Error doProcess(const BinaryVector<T>& value)
            {
                const auto size = value.size();
                PODS_SAFE_CALL(checkSize(size));
                return format_.saveBlob(value.data(), static_cast<Size>(size));
            }

            template <class T, size_t ArraySize>
            Error doProcess(const T(&value)[ArraySize])
            {
                return saveArray<const T>(value, ArraySize);
            }

            template <class T, size_t ArraySize>
            Error doProcess(const std::array<T, ArraySize>& value)
            {
                return saveArray<const T>(value.data(), ArraySize);
            }

            template <class T>
            Error doProcess(const std::vector<T>& value)
            {
                return saveArray<const T>(value.data(), value.size());
            }

            template <class T>
            Error doProcess(const std::deque<T>& value)
            {
                return saveArray<const T>(value.cbegin(), value.size());
            }

            template <class T>
            Error doProcess(const std::forward_list<T>& value)
            {
                size_t size = 0;
                std::for_each(value.begin(), value.end(), [&](const T&) { ++size; });
                return saveArray<const T>(value.cbegin(), size);
            }

            template <class T>
            Error doProcess(const std::list<T>& value)
            {
                return saveArray<const T>(value.cbegin(), value.size());
            }

            template <class Key, class Val>
            Error doProcess(const std::pair<Key, Val>& value)
            {
                PODS_SAFE_CALL(format_.startObject());

                PODS_SAFE_CALL(format_.saveName(PODS_KEY));
                PODS_SAFE_CALL(doProcess(value.first));

                PODS_SAFE_CALL(format_.saveName(PODS_VAL));
                PODS_SAFE_CALL(doProcess(value.second));

                return format_.endObject();
            }

            template <class Key, class Val>
            Error doProcess(const std::map<Key, Val>& value)
            {
                return saveMap<const Key, const Val>(value.cbegin(), value.size());
            }

            template <class T, class Iterator, typename std::enable_if<IsPodsSerializable<T>::value, int>::type = 0>
            Error saveArray(Iterator&& begin, size_t size)
            {
                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(saveVersion<T>(PODS_VERSION));
                PODS_SAFE_CALL(doSaveArray(std::forward<Iterator>(begin), size));
                return format_.endObject();
            }

            template <class T, class Iterator, typename std::enable_if<!IsPodsSerializable<T>::value, int>::type = 0>
            Error saveArray(Iterator&& begin, size_t size)
            {
                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(doSaveArray(std::forward<Iterator>(begin), size));
                return format_.endObject();
            }

            template <class Key, class Val, class Iterator,
                typename std::enable_if<IsPodsSerializable<Key>::value && IsPodsSerializable<Val>::value, int>::type = 0>
            Error saveMap(Iterator&& begin, size_t size)
            {
                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(saveVersion<Key>(PODS_KEY_VERSION));
                PODS_SAFE_CALL(saveVersion<Val>(PODS_VAL_VERSION));
                PODS_SAFE_CALL(doSaveMap(std::forward<Iterator>(begin), size));
                return format_.endObject();
            }

            template <class Key, class Val, class Iterator,
                typename std::enable_if<IsPodsSerializable<Key>::value && !IsPodsSerializable<Val>::value, int>::type = 0>
            Error saveMap(Iterator&& begin, size_t size)
            {
                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(saveVersion<Key>(PODS_KEY_VERSION));
                PODS_SAFE_CALL(doSaveMap(std::forward<Iterator>(begin), size));
                return format_.endObject();
            }

            template <class Key, class Val, class Iterator,
                typename std::enable_if<!IsPodsSerializable<Key>::value && IsPodsSerializable<Val>::value, int>::type = 0>
            Error saveMap(Iterator&& begin, size_t size)
            {
                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(saveVersion<Val>(PODS_VAL_VERSION));
                PODS_SAFE_CALL(doSaveMap(std::forward<Iterator>(begin), size));
                return format_.endObject();
            }

            template <class Key, class Val, class Iterator,
                typename std::enable_if<!IsPodsSerializable<Key>::value && !IsPodsSerializable<Val>::value, int>::type = 0>
            Error saveMap(Iterator&& begin, size_t size)
            {
                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(doSaveMap(std::forward<Iterator>(begin), size));
                return format_.endObject();
            }

            template <class Iterator>
            Error doSaveArray(Iterator begin, size_t size)
            {
                PODS_SAFE_CALL(checkSize(size));
                PODS_SAFE_CALL(format_.startArray(static_cast<Size>(size)));

                for (size_t i = 0; i < size; ++i)
                {
                    PODS_SAFE_CALL(doProcessWithoutVersion(*begin++));
                }

                return format_.endArray();
            }

            template <class Iterator>
            Error doSaveMap(Iterator begin, size_t size)
            {
                PODS_SAFE_CALL(checkSize(size));
                PODS_SAFE_CALL(format_.startMap(static_cast<Size>(size)));

                for (size_t i = 0; i < size; ++i)
                {
                    PODS_SAFE_CALL(doProcessWithoutVersion(*begin++));
                }

                return format_.endMap();
            }

            template <class T>
            Error saveVersion(const char* name)
            {
                static_assert(std::is_same<Version, decltype(T::version())>::value,
                    "The return value of version() must be a pods::Version");
                PODS_SAFE_CALL(format_.saveName(name));
                return format_.save(T::version());
            }

        private:
            Format format_;
        };
    }
}
