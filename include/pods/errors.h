#pragma once

#include <cstdint>

namespace pods
{
    enum class Error : uint32_t
    {
        NoError,

        MandatoryFieldMissed,
        OptionalFieldMissed,
        CorruptedArchive,
        NotEnoughMemory,
        UnexpectedEnd,
        InvalidSize,
        WriteError,
        ReadError,
        Eof,
        EndOfArray,
        EndOfObject,

        UnknownError
    };
}
