#pragma once

#include <algorithm>
#include <cassert>
#include <cstring>
#include <limits>
#include <memory>
#include <type_traits>

#include "details/settings.h"
#include "details/utils.h"

#include "errors.h"
#include "types.h"

namespace pods
{
    class InputBuffer
    {
        friend class InputStream;
    public:
        explicit InputBuffer(size_t size)
            : allocated_(static_cast<char*>(malloc(size)))
            , maxSize_(size)
            , data_(allocated_)
            , pos_(0)
        {
            assert(size > 0);

            if (allocated_ == nullptr)
            {
                throw std::bad_alloc();
            }
        }

        InputBuffer(const char* data, size_t size) noexcept
            : allocated_(nullptr)
            , maxSize_(size)
            , data_(data)
            , pos_(0)
        {
            assert(size > 0);
        }

        ~InputBuffer()
        {
            if (allocated_ != nullptr)
            {
                free(allocated_);
            }
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
            return get(reinterpret_cast<char*>(&value), sizeof(T));
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
        void gotoEnd() noexcept
        {
            pos_ = maxSize_;
        }

        void reset(size_t newSize) noexcept
        {
            assert(newSize <= maxSize_);
            maxSize_ = newSize;
            pos_ = 0;
        }

        char* data() const noexcept
        {
            return allocated_;
        }

        size_t maxSize() const noexcept
        {
            return maxSize_;
        }

        size_t available() const noexcept
        {
            return maxSize_ - pos_;
        }

    private:
        char* allocated_;
        size_t maxSize_;
        const char* data_;
        size_t pos_;
    };

    class OutputBuffer final
    {
    public:
        explicit OutputBuffer(size_t size)
            : data_(static_cast<char*>(malloc(size)))
            , begin_(data_)
            , current_(data_)
            , end_(data_ + size)
        {
            if (data_ == nullptr)
            {
                throw std::bad_alloc();
            }
        }

        OutputBuffer(char* data, size_t size) noexcept
            : data_(nullptr)
            , begin_(data)
            , current_(data)
            , end_(data + size)
        {
        }

        ~OutputBuffer()
        {
            if (data_ != nullptr)
            {
                free(data_);
            }
        }

        OutputBuffer(const OutputBuffer&) = delete;
        OutputBuffer& operator=(const OutputBuffer&) = delete;

        OutputBuffer(OutputBuffer&&) = delete;
        OutputBuffer& operator=(OutputBuffer&&) = delete;

        Error put(bool value)
        {
            return put(value ? True : False);
        }

        template <class T, typename std::enable_if<sizeof(T) == 1, int>::type = 0>
        Error put(T value)
        {
            auto to = current_;
            current_ += sizeof(T);
            if (current_ <= end_)
            {
                *to = static_cast<char>(value);
                return Error::NoError;
            }
            current_ = to;
            return Error::NotEnoughMemory;
        }

        template <class T, typename std::enable_if<sizeof(T) != 1, int>::type = 0>
        Error put(T value)
        {
            return put(reinterpret_cast<char*>(&value), sizeof(T));
        }

        Error put(const char* data, size_t size)
        {
            auto to = current_;
            current_ += size;
            if (current_ <= end_)
            {
                memcpy(to, data, size);
                return Error::NoError;
            }
            current_ = to;
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
            return begin_;
        }

        size_t size() const noexcept
        {
            assert(begin_ <= current_);
            return static_cast<size_t>(current_ - begin_);
        }

        size_t available() const noexcept
        {
            assert(current_ <= end_);
            return static_cast<size_t>(end_ - current_);
        }

        void clear() noexcept
        {
            current_ = begin_;
        }

        void flush() noexcept
        {
        }

    private:
        char* const data_;

        char* const begin_;
        char* current_;
        char* const end_;
    };

    class ResizableOutputBuffer final
    {
    public:
        explicit ResizableOutputBuffer(
            size_t initialSize = details::PrefferedBufferSize,
            size_t maxSize = std::numeric_limits<uint32_t>::max()) noexcept
            : maxSize_(maxSize)
            , data_(static_cast<char*>(malloc(initialSize)))
            , current_(data_)
            , available_(initialSize)
        {
            assert(initialSize <= maxSize);
        }

        ~ResizableOutputBuffer()
        {
            free(data_);
        }

        ResizableOutputBuffer(const ResizableOutputBuffer&) = delete;
        ResizableOutputBuffer& operator=(const ResizableOutputBuffer&) = delete;

        ResizableOutputBuffer(ResizableOutputBuffer&&) = delete;
        ResizableOutputBuffer& operator=(ResizableOutputBuffer&&) = delete;

        Error put(bool value)
        {
            return put(value ? True : False);
        }

        template <class T, typename std::enable_if<sizeof(T) == 1, int>::type = 0>
        Error put(T value)
        {
            auto to = getPtr(sizeof(T));
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
            return put(reinterpret_cast<char*>(&value), sizeof(T));
        }

        Error put(const char* data, size_t size)
        {
            assert(size <= std::numeric_limits<Size>::max()); // is checked in the serializer

            auto to = getPtr(static_cast<Size>(size));
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
            assert(size <= std::numeric_limits<Size>::max());
            const auto totalSize = size * sizeof(T);
            return put(reinterpret_cast<const char*>(data), totalSize);
        }

        const char* data() const noexcept
        {
            return data_;
        }

        size_t size() const noexcept
        {
            assert(data_ <= current_);
            return static_cast<size_t>(current_ - data_);
        }

        size_t capacity() const noexcept
        {
            return size() + available_;
        }

        void clear() noexcept
        {
            available_ += size();
            current_ = data_;
        }

        void flush() noexcept
        {
        }

    private:
        char* getPtr(Size size) noexcept
        {
            if (size <= available_)
            {
                available_ -= size;
                auto ptr = current_;
                current_ += size;
                return ptr;
            }

            const auto used = this->size();
            if (used + size <= maxSize_)
            {
                const auto newSize = std::min<size_t>(maxSize_, (used + size) * 2);
                auto newPtr = realloc(data_, newSize);
                if (newPtr == nullptr)
                {
                    return nullptr;
                }

                data_ = static_cast<char*>(newPtr);
                available_ = newSize - used - size;
                auto ptr = data_ + used;
                current_ = ptr + size;
                return ptr;
            }

            return nullptr;
        }

    private:
        const size_t maxSize_;

        char* data_;
        char* current_;
        size_t available_;
    };
}
