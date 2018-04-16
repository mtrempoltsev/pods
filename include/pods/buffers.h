#pragma once

#include <cassert>
#include <cstring>
#include <limits>
#include <type_traits>

#include "details/memory_managers.h"
#include "details/settings.h"
#include "details/utils.h"

#include "errors.h"
#include "types.h"

namespace pods
{
    class InputBuffer
    {
    public:
        InputBuffer(const char* data, size_t size) noexcept
            : maxSize_(size)
            , data_(data)
            , pos_(0)
        {
            assert(size > 0);
        }

        InputBuffer(const InputBuffer&) = delete;
        InputBuffer& operator=(const InputBuffer&) = delete;

        InputBuffer(InputBuffer&&) = delete;
        InputBuffer& operator=(InputBuffer&&) = delete;

        Error get(bool& value) noexcept
        {
            Bool tmp = False;

            PODS_SAFE_CALL(get(tmp));

            switch (tmp)
            {
            case False:
                value = false;
                return Error::NoError;
            case True:
                value = true;
                return Error::NoError;
            }

            return Error::CorruptedArchive;
        }

        template <class T>
        Error get(T& value, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 1, int>::type = 0) noexcept
        {
            if (pos_ + sizeof(T) <= maxSize_)
            {
                value = static_cast<T>(data_[pos_]);
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

    protected:
        InputBuffer() noexcept
            : maxSize_(0)
            , data_(nullptr)
            , pos_(0)
        {
        }

        void setBuffer(const char* data, size_t size)
        {
            maxSize_ = size;
            data_ = data;
        }

    protected:
        size_t maxSize_;
        const char* data_;
        size_t pos_;
    };

    namespace details
    {
        template <class MemoryManager>
        class OutputBufferBase
        {
        public:
            explicit OutputBufferBase(MemoryManager&& memoryManager) noexcept
                : memoryManager_(std::forward<MemoryManager>(memoryManager))
            {
            }

            OutputBufferBase(const OutputBufferBase<MemoryManager>&) = delete;
            OutputBufferBase<MemoryManager>& operator=(const OutputBufferBase<MemoryManager>&) = delete;

            OutputBufferBase(OutputBufferBase<MemoryManager>&&) = delete;
            OutputBufferBase<MemoryManager>& operator=(OutputBufferBase<MemoryManager>&&) = delete;

            Error put(bool value)
            {
                auto to = memoryManager_.getPtr(sizeof(Bool));
                if (to != nullptr)
                {
                    *to = value ? True : False;
                    return Error::NoError;
                }

                return Error::NotEnoughMemory;
            }

            template <class T, typename std::enable_if<sizeof(T) == 1, int>::type = 0>
            Error put(T value)
            {
                auto to = memoryManager_.getPtr(sizeof(T));
                if (to != nullptr)
                {
                    *to = static_cast<char>(value);
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

            void clear() noexcept
            {
                memoryManager_.reset();
            }

            size_t available() const noexcept
            {
                return memoryManager_.available();
            }

            void flush() noexcept
            {
            }

        private:
            MemoryManager memoryManager_;
        };
    }

    class OutputBuffer final
        : public details::OutputBufferBase<details::FixedSizeMemoryManager>
    {
    public:
        explicit OutputBuffer(size_t size) noexcept
            : details::OutputBufferBase<details::FixedSizeMemoryManager>(details::FixedSizeMemoryManager(size))
        {
        }

        OutputBuffer(char* data, size_t size) noexcept
            : details::OutputBufferBase<details::FixedSizeMemoryManager>(details::FixedSizeMemoryManager(data, size))
        {
        }
    };

    class ResizableOutputBuffer final
        : public details::OutputBufferBase<details::ResizeableMemoryManager>
    {
    public:
        explicit ResizableOutputBuffer(
            size_t initialSize = details::PrefferedBufferSize,
            size_t maxSize = std::numeric_limits<uint32_t>::max())
            : details::OutputBufferBase<details::ResizeableMemoryManager>(
                details::ResizeableMemoryManager(initialSize, maxSize))
        {
            assert(initialSize <= maxSize);
        }
    };
}
