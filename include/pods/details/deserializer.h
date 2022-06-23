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
#include "utils.h"

#include "../errors.h"
#include "../types.h"

namespace pods
{
    namespace details
    {
        template <class Format, class Storage>
        class Deserializer final
        {
        public:
            explicit Deserializer(Storage& storage) noexcept
                : format_(storage)
            {
            }

            Deserializer(const Deserializer<Format, Storage>&) = delete;
            Deserializer& operator=(const Deserializer<Format, Storage>&) = delete;

            template <class T>
            Error load(T& data)
            {
                return deserialize(data);
            }

            Error operator()() noexcept
            {
                return Error::NoError;
            }

            template <class... ArgsT>
            Error operator()(ArgsT&&... args)
            {
                return processField(args...);
            }

        private:
            template <class T>
            Error deserialize(T& value)
            {
                static_assert(IsPodsSerializable<T, decltype(this)>::value,
                    "You must define the serialize() method for each serializable struct");

                PODS_SAFE_CALL(format_.startDeserialization());
                PODS_SAFE_CALL(value.serialize(*this));
                return format_.endDeserialization();
            }

            template <class T>
            Error processField(const char* name, T& value)
            {
                const auto result = format_.checkName(name);
                if (result == Error::NoError)
                {
                    return processValue(value);
                }

                return result == Error::OptionalFieldMissed
                    ? Error::NoError
                    : result;
            }

            template <class T, class... ArgsT>
            Error processField(const char* name, T& value, ArgsT&... args)
            {
                const auto result = format_.checkName(name);
                if (result == Error::NoError)
                {
                    const auto error = processValue(value);
                    return error == Error::NoError
                        ? processField(args...)
                        : error;
                }

                return result == Error::OptionalFieldMissed
                    ? processField(args...)
                    : result;
            }

            template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error processValue(T& value)
            {
                return deserialize(value);
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
            Error processValue(T& value)
            {
                return format_.load(value);
            }

            template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
            Error processValue(T& value)
            {
                std::underlying_type_t<T> n = 0;
                PODS_SAFE_CALL(format_.load(n));
                value = static_cast<T>(n);
                return Error::NoError;
            }

            Error processValue(std::string& value)
            {
                return format_.load(value);
            }

            Error processValue(BinaryArray& value)
            {
                return format_.loadBlob(value.data(), value.size());
            }

            template <class T>
            Error processValue(BinaryVector<T>& value)
            {
                return format_.loadBlob(
                    [&](char*& data, Size size)
                    {
                        return value.allocate(data, size);
                    });
            }

            template <class T, size_t ArraySize>
            Error processValue(T(&value)[ArraySize])
            {
                return loadArray(value, ArraySize);
            }

            template <class T, size_t ArraySize>
            Error processValue(std::array<T, ArraySize>& value)
            {
                return loadArray(value.data(), ArraySize);
            }

            template <class T>
            Error loadArray(T* begin, size_t arraySize)
            {
                if constexpr (Format::Traits::SupportsSeqSizes)
                {
                    Size size = 0;
                    PODS_SAFE_CALL(format_.startArray(size));

                    if (size != arraySize)
                    {
                        return Error::CorruptedArchive;
                    }

                    for (Size i = 0; i < size; ++i)
                    {
                        PODS_SAFE_CALL(processValue(*begin++));
                    }

                    return format_.endArray();
                }
                else
                {
                    PODS_SAFE_CALL(format_.startArray());

                    for (Size i = 0; i < MaxSize; ++i)
                    {
                        const Error error = processValue(*begin++);
                        if (error == Error::NoError)
                        {
                            continue;
                        }
                        else if (error == Error::EndOfArray)
                        {
                            return i == arraySize
                                ? Error::NoError
                                : Error::CorruptedArchive;
                        }
                        else
                        {
                            return Error::CorruptedArchive;
                        }
                    }

                    return Error::InvalidSize;
                }
            }

            template <class T>
            Error processValue(std::vector<T>& value)
            {
                return loadSequense(value);
            }

            template <class T>
            Error processValue(std::deque<T>& value)
            {
                return loadSequense(value);
            }

            template <class T>
            Error processValue(std::forward_list<T>& value)
            {
                if constexpr (Format::Traits::SupportsSeqSizes)
                {
                    Size size = 0;
                    PODS_SAFE_CALL(format_.startArray(size));

                    value.clear();

                    for (Size i = 0; i < size; ++i)
                    {
                        T temp;
                        PODS_SAFE_CALL(processValue(temp));
                        value.push_front(std::move(temp));
                    }

                    value.reverse();

                    return format_.endArray();
                }
                else
                {
                    PODS_SAFE_CALL(format_.startArray());

                    value.clear();

                    for (Size i = 0; i < MaxSize; ++i)
                    {
                        T temp;
                        const auto error = processValue(temp);
                        if (error == Error::NoError)
                        {
                            value.push_front(std::move(temp));
                        }
                        else if (error == Error::EndOfArray)
                        {
                            value.reverse();
                            return Error::NoError;
                        }
                        else
                        {
                            return error;
                        }
                    }

                    return Error::InvalidSize;
                }
            }

            template <class T>
            Error processValue(std::list<T>& value)
            {
                if constexpr (Format::Traits::SupportsSeqSizes)
                {
                    Size size = 0;
                    PODS_SAFE_CALL(format_.startArray(size));

                    value.clear();

                    for (Size i = 0; i < size; ++i)
                    {
                        T temp;
                        PODS_SAFE_CALL(processValue(temp));
                        value.push_back(std::move(temp));
                    }

                    return format_.endArray();
                }
                else
                {
                    PODS_SAFE_CALL(format_.startArray());

                    value.clear();

                    for (Size i = 0; i < MaxSize; ++i)
                    {
                        T temp;
                        const auto error = processValue(temp);
                        if (error == Error::NoError)
                        {
                            value.push_back(std::move(temp));
                        }
                        else if (error == Error::EndOfArray)
                        {
                            return Error::NoError;
                        }
                        else
                        {
                            return error;
                        }
                    }

                    return Error::InvalidSize;
                }
            }

            template <class Container>
            Error loadSequense(Container& value)
            {
                if constexpr (Format::Traits::SupportsSeqSizes)
                {
                    Size size = 0;
                    PODS_SAFE_CALL(format_.startArray(size));

                    value.resize(size);

                    auto it = value.begin();
                    for (Size i = 0; i < size; ++i)
                    {
                        PODS_SAFE_CALL(processValue(*it++));
                    }

                    return format_.endArray();
                }
                else
                {
                    PODS_SAFE_CALL(format_.startArray());

                    value.clear();

                    for (Size i = 0; i < MaxSize; ++i)
                    {
                        typename Container::value_type temp;
                        const auto error = processValue(temp);
                        if (error == Error::NoError)
                        {
                            value.push_back(std::move(temp));
                            continue;
                        }
                        else if (error == Error::EndOfArray)
                        {
                            return Error::NoError;
                        }
                        else
                        {
                            return error;
                        }
                    }

                    return Error::InvalidSize;
                }
            }

            template <class Key, class Val>
            Error processValue(std::pair<Key, Val>& value)
            {
                PODS_SAFE_CALL(format_.startArray(2));

                PODS_SAFE_CALL(processValue(value.first));
                PODS_SAFE_CALL(processValue(value.second));

                return format_.endArray();
            }

            template <class Key, class Val>
            Error processValue(std::map<Key, Val>& value)
            {
                static_assert(!(Format::Traits::OnlyStringKeys && !std::is_same<std::string, std::remove_const_t<Key>> ::value),
                    "This serialization format only supports string keys");

                if constexpr (Format::Traits::SupportsSeqSizes)
                {
                    Size size = 0;
                    PODS_SAFE_CALL(format_.startMap(size));

                    value.clear();

                    auto hint = value.begin();
                    for (Size i = 0; i < size; ++i)
                    {
                        Key key;
                        if constexpr (Format::Traits::OnlyStringKeys)
                        {
                            PODS_SAFE_CALL(format_.loadKey(key));
                        }
                        else
                        {
                            PODS_SAFE_CALL(processValue(key));
                        }

                        Val val;
                        PODS_SAFE_CALL(processValue(val));

                        hint = value.emplace_hint(hint, std::move(key), std::move(val));
                    }

                    return format_.endMap();
                }
                else
                {
                    PODS_SAFE_CALL(format_.startMap());

                    value.clear();

                    auto hint = value.begin();
                    for (Size i = 0; i < MaxSize; ++i)
                    {
                        Error error = Error::NoError;

                        Key key;
                        if constexpr (Format::Traits::OnlyStringKeys)
                        {
                            error = format_.loadKey(key);
                        }
                        else
                        {
                            error = processValue(key);
                        }

                        if (error == Error::NoError)
                        {
                            Val val;
                            PODS_SAFE_CALL(processValue(val));

                            hint = value.emplace_hint(hint, std::move(key), std::move(val));
                        }
                        else if (error == Error::EndOfArray)
                        {
                            return Error::NoError;
                        }
                        else if (error == Error::EndOfObject)
                        {
                            return Error::NoError;
                        }
                        else
                        {
                            return error;
                        }
                    }

                    return Error::InvalidSize;
                }
            }

        private:
            Format format_;
        };
    }
}
