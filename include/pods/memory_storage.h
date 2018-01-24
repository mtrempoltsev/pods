#pragma once

#include <cassert>
#include <limits>
#include <type_traits>

#include "details/memory_managers.h"

#include "errors.h"
#include "types.h"

namespace pods
{
    class ReadOnlyMemoryStorage final
    {
    public:
        ReadOnlyMemoryStorage(const char* data, size_t size) noexcept
            : maxSize_(size)
            , data_(data)
            , pos_(0)
        {
            assert(size > 0);
        }

        ReadOnlyMemoryStorage(const ReadOnlyMemoryStorage&) = delete;
        ReadOnlyMemoryStorage& operator=(const ReadOnlyMemoryStorage&) = delete;

        ReadOnlyMemoryStorage(ReadOnlyMemoryStorage&&) = delete;
        ReadOnlyMemoryStorage& operator=(ReadOnlyMemoryStorage&&) = delete;

        template <class T>
        Error get(T& value, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 1, int>::type = 0) noexcept
        {
            if (pos_ + sizeof(T) <= maxSize_)
            {
                value = data_[pos_];
                pos_ += sizeof(T);
                return Error::NoError;
            }

            return Error::UnexpectedEnd;
        }

        template <class T>
        Error get(T& value, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) != 1, int>::type = 0) noexcept
        {
            if (pos_ + sizeof(T) <= maxSize_)
            {
                auto to = reinterpret_cast<char*>(&value);
                auto from = data_ + pos_;
                memcpy(to, from, sizeof(T));
                pos_ += sizeof(T);
                return Error::NoError;
            }

            return Error::UnexpectedEnd;
        }

        Error get(char* data, size_t size) noexcept
        {
            if (pos_ + size <= maxSize_)
            {
                auto begin = data_ + pos_;
                memcpy(data, begin, size);
                pos_ += size;
                return Error::NoError;
            }

            return Error::UnexpectedEnd;
        }

        template <class T>
        Error get(T* data, size_t size) noexcept
        {
            const auto totalSize = size * sizeof(T);
            return get(reinterpret_cast<char*>(data), totalSize);
        }

    private:
        const size_t maxSize_;
        const char* const data_;
        size_t pos_;
    };

    namespace details
    {
        template <class MemoryManager>
        class WriteOnlyMemoryStorage
        {
        public:
            explicit WriteOnlyMemoryStorage(MemoryManager&& memoryManager) noexcept
                : memoryManager_(std::forward<MemoryManager>(memoryManager))
            {
            }

            WriteOnlyMemoryStorage(const WriteOnlyMemoryStorage<MemoryManager>&) = delete;
            WriteOnlyMemoryStorage<MemoryManager>& operator=(const WriteOnlyMemoryStorage<MemoryManager>&) = delete;

            WriteOnlyMemoryStorage(WriteOnlyMemoryStorage<MemoryManager>&&) = delete;
            WriteOnlyMemoryStorage<MemoryManager>& operator=(WriteOnlyMemoryStorage<MemoryManager>&&) = delete;

            template <class T, typename std::enable_if<sizeof(T) == 1, int>::type = 0>
            Error put(T value)
            {
                auto to = memoryManager_.getPtr(sizeof(T));
                if (to != nullptr)
                {
                    *to = value;
                    return Error::NoError;
                }

                return Error::NotEnoughMemory;
            }

            template <class T, typename std::enable_if<sizeof(T) != 1, int>::type = 0>
            Error put(T value)
            {
                auto to = memoryManager_.getPtr(sizeof(T));
                if (to != nullptr)
                {
                    auto from = reinterpret_cast<char*>(&value);
                    memcpy(to, from, sizeof(T));
                    return Error::NoError;
                }

                return Error::NotEnoughMemory;
            }

            Error put(const char* data, size_t size)
            {
                auto to = memoryManager_.getPtr(size);
                if (to != nullptr)
                {
                    memcpy(to, data, size);
                    return Error::NoError;
                }

                return Error::NotEnoughMemory;
            }

            template <class T>
            Error put(const T* data, size_t size)
            {
                const auto totalSize = size * sizeof(T);
                return put(reinterpret_cast<const char*>(data), totalSize);
            }

            const char* data() const noexcept
            {
                return memoryManager_.data();
            }

            size_t size() const noexcept
            {
                return memoryManager_.size();
            }

        private:
            MemoryManager memoryManager_;
        };
    }

    class FixedSizeWriteOnlyMemoryStorage final
        : public details::WriteOnlyMemoryStorage<details::FixedSizeMemoryManager>
    {
    public:
        explicit FixedSizeWriteOnlyMemoryStorage(size_t size) noexcept
            : details::WriteOnlyMemoryStorage<details::FixedSizeMemoryManager>(details::FixedSizeMemoryManager(size))
        {
        }
    };

    class ResizeableWriteOnlyMemoryStorage final
        : public details::WriteOnlyMemoryStorage<details::ResizeableMemoryManager>
    {
    public:
        explicit ResizeableWriteOnlyMemoryStorage(
            size_t initialSize = PrefferedBufferSize,
            size_t maxSize = std::numeric_limits<uint32_t>::max())
            : details::WriteOnlyMemoryStorage<details::ResizeableMemoryManager>(
                details::ResizeableMemoryManager(initialSize, maxSize))
        {
            assert(initialSize <= maxSize);
        }
    };
}
