#pragma once

#include <cstdint>

namespace pods
{
    enum class Error : uint32_t
    {
        NoError,

        ArchiveVersionMismatch,
        NotEnoughMemory,
        UnexpectedEnd,
        SizeToLarge,

        UnknownError
    };
}
