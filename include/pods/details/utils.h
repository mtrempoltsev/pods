#pragma once

namespace pods
{
    namespace details
    {

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
