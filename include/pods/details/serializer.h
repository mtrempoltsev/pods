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
#include "serialization_traits.h"
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
                return processField(std::forward<ArgsT>(args)...);
            }

        private:
            template <class T>
            Error serialize(const T& value)
            {
                using TTT = decltype(T().serialize(*static_cast<decltype(this)>(nullptr)));
                static_assert(std::is_same<TTT, Error>::value, "OLOLO");
                static_assert(IsPodsSerializable<T, decltype(this)>::value,
                    "You must define the serialize() method for each serializable struct");

                PODS_SAFE_CALL(format_.startSerialization());
                PODS_SAFE_CALL(const_cast<T&>(value).serialize(*this));
                return format_.endSerialization();
            }

            template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error processField(const char* name, const T& value)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                return processValue(value);
            }

            template <class T, class... ArgsT, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error processField(const char* name, const T& value, ArgsT&&... args)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                const auto error = processValue(value);
                return error == Error::NoError
                    ? processField(std::forward<ArgsT>(args)...)
                    : error;
            }

            template <class T, size_t ArraySize>
            Error processField(const char* name, const T(&value)[ArraySize])
            {
                PODS_SAFE_CALL(format_.saveName(name));
                return processValue(value);
            }

            template <class T, size_t ArraySize, class... ArgsT>
            Error processField(const char* name, const T(&value)[ArraySize], ArgsT&&... args)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                const auto error = processValue(value);
                return error == Error::NoError
                    ? processField(std::forward<ArgsT>(args)...)
                    : error;
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, int>::type = 0>
            Error processField(const char* name, T value)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                return processValue(value);
            }

            template <class T, class... ArgsT, typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, int>::type = 0>
            Error processField(const char* name, T value, ArgsT&&... args)
            {
                PODS_SAFE_CALL(format_.saveName(name));
                const auto error = processValue(value);
                return error == Error::NoError
                    ? processField(std::forward<ArgsT>(args)...)
                    : error;
            }

            template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error processValue(const T& value)
            {
                return serialize(value);
            }

            template <class T, size_t ArraySize>
            Error processValue(const T(&value)[ArraySize])
            {
                return saveArray(value, ArraySize);
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
            Error processValue(T value)
            {
                return format_.save(value);
            }

            template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
            Error processValue(T value)
            {
                return format_.save(std::underlying_type_t<T>(value));
            }

            Error processValue(const std::string& value)
            {
                PODS_SAFE_CALL(checkSize(value.size()));
                return format_.save(value);
            }

            Error processValue(const BinaryArray& value)
            {
                const auto size = value.size();
                PODS_SAFE_CALL(checkSize(size));
                return format_.saveBlob(value.data(), static_cast<Size>(size));
            }

            template <class T>
            Error processValue(const BinaryVector<T>& value)
            {
                const auto size = value.size();
                PODS_SAFE_CALL(checkSize(size));
                return format_.saveBlob(value.data(), static_cast<Size>(size));
            }

            template <class T, size_t ArraySize>
            Error processValue(const std::array<T, ArraySize>& value)
            {
                return saveArray(value.data(), ArraySize);
            }

            template <class T>
            Error processValue(const std::vector<T>& value)
            {
                return saveArray(value.data(), value.size());
            }

            template <class T>
            Error processValue(const std::deque<T>& value)
            {
                return saveArray(value.cbegin(), value.size());
            }

            template <class T>
            Error processValue(const std::forward_list<T>& value)
            {
                size_t size = 0;
                std::for_each(value.begin(), value.end(), [&](const T&) { ++size; });
                return saveArray(value.cbegin(), size);
            }

            template <class T>
            Error processValue(const std::list<T>& value)
            {
                return saveArray(value.cbegin(), value.size());
            }

            template <class Key, class Val>
            Error processValue(const std::pair<Key, Val>& value)
            {
                PODS_SAFE_CALL(format_.startArray(2));

                PODS_SAFE_CALL(processValue(value.first));
                PODS_SAFE_CALL(processValue(value.second));

                return format_.endArray();
            }

            template <class Key, class Val>
            Error processValue(const std::map<Key, Val>& value)
            {
                static_assert(!(Format::Traits::OnlyStringKeys && !std::is_same<std::string, std::remove_const_t<Key>> ::value),
                    "This serialization format only supports string keys");

                const auto size = value.size();

                PODS_SAFE_CALL(checkSize(size));
                PODS_SAFE_CALL(format_.startMap(static_cast<Size>(size)));

                for (const auto& v : value)
                {
                    if constexpr (Format::Traits::OnlyStringKeys)
                    {
                        PODS_SAFE_CALL(format_.saveKey(v.first.c_str()));
                    }
                    else
                    {
                        PODS_SAFE_CALL(processValue(v.first));
                    }
                    PODS_SAFE_CALL(processValue(v.second));
                }

                return format_.endMap();
            }

            template <class Iterator>
            Error saveArray(Iterator begin, size_t size)
            {
                PODS_SAFE_CALL(checkSize(size));
                PODS_SAFE_CALL(format_.startArray(static_cast<Size>(size)));

                for (size_t i = 0; i < size; ++i)
                {
                    PODS_SAFE_CALL(processValue(*begin++));
                }

                return format_.endArray();
            }

        private:
            Format format_;
        };
    }
}
