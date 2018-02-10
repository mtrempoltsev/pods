﻿#pragma once

#include <type_traits>

#include "../types.h"

namespace pods
{
    namespace details
    {
        template <class T>
        struct IsPodsSerializableImpl
        {
        private:
            static void check(...);

            template <class U>
            static decltype(U::version()) check(U*);

        public:
            static constexpr bool value =
                std::is_same<Version, decltype(IsPodsSerializableImpl<T>::check(static_cast<std::remove_reference_t<T>*>(nullptr)))>::value;
        };

        template <class T>
        struct IsPodsSerializable
            : std::integral_constant<bool, IsPodsSerializableImpl<T>::value>
        {
        };
    }

    constexpr Size toSize(size_t value)
    {
        return static_cast<Size>(value);
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