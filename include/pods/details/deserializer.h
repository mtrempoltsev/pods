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
                return process(args...);
            }

        private:
            template <class T>
            Error deserialize(T& value)
            {
                static_assert(IsPodsSerializable<T>::value,
                    "You must define the methods version() and serialize() for each serializable struct");

                PODS_SAFE_CALL(format_.startDeserialization());
                Version version = NoVersion;
                PODS_SAFE_CALL(loadVersion<T>(PODS_VERSION, version));
                PODS_SAFE_CALL(value.serialize(*this, version));
                return format_.endDeserialization();
            }

            template <class T>
            Error doDeserialize(T& value)
            {
                static_assert(IsPodsSerializable<T>::value,
                    "You must define the methods version() and serialize() for each serializable struct");

                PODS_SAFE_CALL(format_.startObject());
                Version version = NoVersion;
                PODS_SAFE_CALL(loadVersion<T>(PODS_VERSION, version));
                PODS_SAFE_CALL(value.serialize(*this, version));
                return format_.endObject();
            }

            template <class T>
            Error doDeserializeWithoutVersion(T& value, Version version)
            {
                static_assert(IsPodsSerializable<T>::value,
                    "You must define the methods version() and serialize() for each serializable struct");

                PODS_SAFE_CALL(format_.startObject());
                PODS_SAFE_CALL(value.serialize(*this, version));
                return format_.endObject();
            }

            template <class T>
            Error process(const char* name, T& value)
            {
                const auto result = format_.checkName(name);
                if (result == Error::NoError)
                {
                    return doProcess(value);
                }
                else if (result == Error::OptionalFieldMissed)
                {
                    return Error::NoError;
                }
                return result;
            }

            template <class T, class... ArgsT>
            Error process(const char* name, T& value, ArgsT&... args)
            {
                const auto result = format_.checkName(name);
                if (result == Error::NoError)
                {
                    const auto error = doProcess(value);
                    return error == Error::NoError
                        ? process(args...)
                        : error;
                }
                else if (result == Error::OptionalFieldMissed)
                {
                    return process(args...);
                }
                return result;
            }

            template <class T, typename std::enable_if<IsPodsSerializable<T>::value, int>::type = 0>
            Error doProcessWithoutVersion(T& value, Version version = NoVersion)
            {
                return doDeserializeWithoutVersion(value, version);
            }

            template <class T, typename std::enable_if<!IsPodsSerializable<T>::value, int>::type = 0>
            Error doProcessWithoutVersion(T& value, Version)
            {
                return doProcess(value);
            }

            template <class Key, class Val>
            Error doProcessWithoutVersion(std::pair<Key, Val>& value, Version keyVersion = NoVersion, Version valVersion = NoVersion)
            {
                PODS_SAFE_CALL(format_.startObject());

                PODS_SAFE_CALL(format_.saveKey(PODS_KEY));
                PODS_SAFE_CALL(doProcessWithoutVersion(value.first, keyVersion));

                PODS_SAFE_CALL(format_.saveKey(PODS_VAL));
                PODS_SAFE_CALL(doProcessWithoutVersion(value.second, valVersion));

                return format_.endObject();
            }

            template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
            Error doProcess(T& value)
            {
                return doDeserialize(value);
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
            Error doProcess(T& value)
            {
                return format_.load(value);
            }

            template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
            Error doProcess(T& value)
            {
                std::underlying_type_t<T> n = 0;
                PODS_SAFE_CALL(format_.load(n));
                value = static_cast<T>(n);
                return Error::NoError;
            }

            Error doProcess(std::string& value)
            {
                return format_.load(value);
            }

            Error doProcess(BinaryArray& value)
            {
                return format_.loadBlob(value.data(), value.size());
            }

            template <class T>
            Error doProcess(BinaryVector<T>& value)
            {
                return format_.loadBlob(
                    [&](char*& data, Size size)
                    {
                        return value.allocate(data, size);
                    });
            }

            template <class T, size_t ArraySize>
            Error doProcess(T(&value)[ArraySize])
            {
                return loadArray(value, ArraySize);
            }

            template <class T, size_t ArraySize>
            Error doProcess(std::array<T, ArraySize>& value)
            {
                return loadArray(value.data(), ArraySize);
            }

            template <class T>
            Error loadArray(T* begin, size_t arraySize)
            {
                return loadArray<T>(
                    [arraySize](Size size)
                    {
                        return size == arraySize
                            ? Error::NoError
                            : Error::CorruptedArchive;
                    },
                    [begin]() { return begin; });
            }

            template <class T>
            Error doProcess(std::vector<T>& value)
            {
                return loadSequenceContainer<T>(value);
            }

            template <class T>
            Error doProcess(std::deque<T>& value)
            {
                return loadSequenceContainer<T>(value);
            }

            template <class T>
            Error doProcess(std::forward_list<T>& value)
            {
                return loadSequenceContainer<T>(value);
            }

            template <class T>
            Error doProcess(std::list<T>& value)
            {
                return loadSequenceContainer<T>(value);
            }

            template <class T, class Container>
            Error loadSequenceContainer(Container& value)
            {
                return loadArray<T>(
                    [&](Size size)
                    {
                        value.resize(size);
                        return Error::NoError;
                    },
                    [&]() { return value.begin(); });
            }

            template <class Key, class Val>
            Error doProcess(std::pair<Key, Val>& value)
            {
                PODS_SAFE_CALL(format_.startObject());

                PODS_SAFE_CALL(format_.checkName(PODS_KEY));
                PODS_SAFE_CALL(doProcess(value.first));

                PODS_SAFE_CALL(format_.checkName(PODS_VAL));
                PODS_SAFE_CALL(doProcess(value.second));

                return format_.endObject();
            }

            template <class Key, class Val>
            Error doProcess(std::map<Key, Val>& value)
            {
                return loadMap<Key, Val>(
                    [](Size) { return Error::NoError; },
                    value);
            }

            template <class T, class Init, class GetIterator, typename std::enable_if<IsPodsSerializable<T>::value, int>::type = 0>
            Error loadArray(Init init, GetIterator getIterator)
            {
                PODS_SAFE_CALL(format_.startObject());

                Version version = NoVersion;
                PODS_SAFE_CALL(loadVersion<T>(PODS_VERSION, version));

                Size size = 0;
                PODS_SAFE_CALL(format_.startArray(size));

                PODS_SAFE_CALL(init(size));

                auto it = getIterator();
                for (Size i = 0; i < size; ++i)
                {
                    PODS_SAFE_CALL(doDeserializeWithoutVersion(*it++, version));
                }

                PODS_SAFE_CALL(format_.endArray());

                return format_.endObject();
            }

            template <class T, class Init, class GetIterator, typename std::enable_if<!IsPodsSerializable<T>::value, int>::type = 0>
            Error loadArray(Init init, GetIterator getIterator)
            {
                PODS_SAFE_CALL(format_.startObject());

                Size size = 0;
                PODS_SAFE_CALL(format_.startArray(size));

                PODS_SAFE_CALL(init(size));

                auto it = getIterator();
                for (Size i = 0; i < size; ++i)
                {
                    PODS_SAFE_CALL(doProcess(*it++));
                }

                PODS_SAFE_CALL(format_.endArray());

                return format_.endObject();
            }

            template <class Key, class Val, class Init, class Container,
                typename std::enable_if<IsPodsSerializable<Key>::value && IsPodsSerializable<Val>::value, int>::type = 0>
            Error loadMap(Init&& init, Container& value)
            {
                PODS_SAFE_CALL(format_.startObject());

                Version keyVersion = NoVersion;
                PODS_SAFE_CALL(loadVersion<Key>(PODS_KEY_VERSION, keyVersion));

                Version valVersion = NoVersion;
                PODS_SAFE_CALL(loadVersion<Val>(PODS_VAL_VERSION, valVersion));

                const auto error = doLoadMap<Key, Val>(std::forward<Init>(init), value, keyVersion, valVersion);
                if (error != Error::NoError)
                {
                    return error;
                }

                return format_.endObject();
            }

            template <class Key, class Val, class Init, class Container,
                typename std::enable_if<IsPodsSerializable<Key>::value && !IsPodsSerializable<Val>::value, int>::type = 0>
            Error loadMap(Init&& init, Container& value)
            {
                PODS_SAFE_CALL(format_.startObject());

                Version valVersion = NoVersion;
                PODS_SAFE_CALL(loadVersion<Key>(PODS_KEY_VERSION, valVersion));

                const auto error = doLoadMap<Key, Val>(std::forward<Init>(init), value, valVersion, NoVersion);
                if (error != Error::NoError)
                {
                    return error;
                }

                return format_.endObject();
            }

            template <class Key, class Val, class Init, class Container,
                typename std::enable_if<!IsPodsSerializable<Key>::value && IsPodsSerializable<Val>::value, int>::type = 0>
            Error loadMap(Init&& init, Container& value)
            {
                PODS_SAFE_CALL(format_.startObject());

                Version valVersion = NoVersion;
                PODS_SAFE_CALL(loadVersion<Val>(PODS_VAL_VERSION, valVersion));

                const auto error = doLoadMap<Key, Val>(std::forward<Init>(init), value, NoVersion, valVersion);
                if (error != Error::NoError)
                {
                    return error;
                }

                return format_.endObject();
            }

            template <class Key, class Val, class Init, class Container,
                typename std::enable_if<!IsPodsSerializable<Key>::value && !IsPodsSerializable<Val>::value, int>::type = 0>
            Error loadMap(Init&& init, Container& value)
            {
                PODS_SAFE_CALL(format_.startObject());

                const auto error = doLoadMap<Key, Val>(std::forward<Init>(init), value, NoVersion, NoVersion);
                if (error != Error::NoError)
                {
                    return error;
                }

                return format_.endObject();
            }

            template <class Key, class Val, class Init, class Container>
            Error doLoadMap(Init&& init, Container& value, Version keyVersion = NoVersion, Version valVersion = NoVersion)
            {
                Size size = 0;
                PODS_SAFE_CALL(format_.startMap(size));

                PODS_SAFE_CALL(init(size));

                auto hint = value.begin();
                for (Size i = 0; i < size; ++i)
                {
                    PODS_SAFE_CALL(format_.startObject());

                    Key key;
                    PODS_SAFE_CALL(format_.checkName(PODS_KEY));
                    PODS_SAFE_CALL(doProcessWithoutVersion(key, keyVersion));

                    Val val;
                    PODS_SAFE_CALL(format_.checkName(PODS_VAL));
                    PODS_SAFE_CALL(doProcessWithoutVersion(val, valVersion));

                    hint = value.emplace_hint(hint, std::move(key), std::move(val));

                    PODS_SAFE_CALL(format_.endObject());
                }

                return format_.endMap();
            }

            template <class T>
            Error loadVersion(const char* name, Version& version)
            {
                PODS_SAFE_CALL(format_.checkName(name));
                PODS_SAFE_CALL(format_.load(version));
                return version > std::decay_t<T>::version()
                    ? Error::ArchiveVersionMismatch
                    : Error::NoError;
            }

        private:
            Format format_;
        };
    }
}
