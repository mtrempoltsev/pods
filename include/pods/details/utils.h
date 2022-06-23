#pragma once

#include <type_traits>

#include "../errors.h"
#include "../types.h"

namespace pods
{
    namespace details
    {
        template<class T, class S, class = T>
        struct IsPodsSerializable
            : std::false_type
        {
        };

        template<class T, class S>
        struct IsPodsSerializable<T, S, std::enable_if_t<std::is_same<decltype(T().serialize(*static_cast<S>(nullptr))), Error>::value, T>>
            : std::true_type
        {
        };

        constexpr bool isOptional(const char* name) noexcept
        {
            return *name == 0;
        }

        constexpr const char* getName(const char* name) noexcept
        {
            return isOptional(name)
                ? name + 1
                : name;
        }

        inline Error checkSize(size_t size) noexcept
        {
            return size < MaxSize
                ? pods::Error::NoError
                : Error::InvalidSize;
        }
    }
}

#ifdef PODS_SAFE_CALL
#error Rename the macro
#endif
#define PODS_SAFE_CALL(foo)                     \
    do                                          \
    {                                           \
        const auto safeCallError = (foo);       \
        if (safeCallError != Error::NoError)    \
        {                                       \
            return safeCallError;               \
        }                                       \
    } while (false)
