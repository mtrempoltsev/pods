#pragma once

#include <cstdint>

namespace spp
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
