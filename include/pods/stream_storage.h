#pragma once

#include <istream>
#include <ostream>

#include "memory_storage.h"

namespace pods
{
    class WriteOnlyStreamStorage final
    {
        class Buffer final
            : public details::WriteOnlyMemoryStorage<details::FixedSizeMemoryManager>
        {
        public:
            FixedSizeWriteOnlyMemoryStorage(char* data, size_t size) noexcept
                : details::WriteOnlyMemoryStorage<details::FixedSizeMemoryManager>(details::FixedSizeMemoryManager(data, size))
            {
            }
        };

    public:
        explicit WriteOnlyStreamStorage(std::ostream& stream)
            : out_(stream)
            , buf_(PrefferedBufferSize, PrefferedBufferSize)
        {
        }

    private:
        std::ostream out_;
        FixedSizeWriteOnlyMemoryStorage buf_;
    };
}
