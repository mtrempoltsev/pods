#pragma once

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
                std::is_same<Version, decltype(IsPodsSerializableImpl<T>::check(static_cast<T*>(nullptr)))>::value;
        };

        template <class T>
        struct IsPodsSerializable
            : std::integral_constant<bool, IsPodsSerializableImpl<T>::value>
        {
        };
    }
}

#ifdef PODS_SAFE_CALL
#error Rename the macro
#endif
#define PODS_SAFE_CALL(foo)              \
    do                                  \
    {                                   \
        const auto error = (foo);       \
        if (error != Error::NoError)    \
        {                               \
            return error;               \
        }                               \
    } while (false)
