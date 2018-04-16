#pragma once

#include <istream>
#include <ostream>

#include "details/settings.h"
#include "details/utils.h"

#include "buffers.h"

namespace pods
{
    class InputStream final
    {
        static constexpr auto BufferSize = details::PrefferedBufferSize;
    public:
        explicit InputStream(std::istream& stream)
            : in_(stream)
            , buffer_(BufferSize)
            , eof_(false)
        {
            buffer_.gotoEnd();
        }

        InputStream(const InputStream&) = delete;
        InputStream& operator=(const InputStream&) = delete;

        InputStream(InputStream&&) = delete;
        InputStream& operator=(InputStream&&) = delete;

        template <class T>
        Error get(T& value) noexcept
        {
            if (buffer_.available() >= sizeof(T))
            {
                return buffer_.get(value);
            }

            const auto bytesLeft = buffer_.available();
            if (bytesLeft > 0)
            {
                auto pos = in_.tellg();
                pos -= static_cast<std::streamsize>(bytesLeft);
                in_.seekg(pos, std::ios_base::beg);
            }

            if (eof_)
            {
                return Error::UnexpectedEnd;
            }

            PODS_SAFE_CALL(readBuffer());

            return buffer_.get(value);
        }

        Error get(char* data, size_t size) noexcept
        {
            if (buffer_.available() >= size)
            {
                return buffer_.get(data, size);
            }

            if (eof_)
            {
                return Error::UnexpectedEnd;
            }

            const auto bytesReaded = buffer_.available();
            const auto needToReed = size - bytesReaded;

            PODS_SAFE_CALL(buffer_.get(data, bytesReaded));

            const auto error = read(data + bytesReaded, needToReed);
            if (error == Error::NoError || error == Error::Eof)
            {
                return Error::NoError;
            }
            return error;
        }

        template <class T>
        Error get(T* data, size_t size) noexcept
        {
            const auto totalSize = size * sizeof(T);
            return get(reinterpret_cast<char*>(data), totalSize);
        }

    private:
        Error read(char* data, size_t size) noexcept
        {
            const bool success = !!in_.read(data, static_cast<std::streamsize>(size));
            if (!success)
            {
                eof_ = in_.eof();
                return eof_
                    ? Error::Eof
                    : Error::ReadError;
            }

            return Error::NoError;
        }

        Error readBuffer() noexcept
        {
            buffer_.gotoBegin();

            const auto error = read(buffer_.mutableData(), BufferSize);

            if (error == Error::Eof)
            {
                const auto bytesReaded = static_cast<size_t>(in_.gcount());
                if (bytesReaded < BufferSize)
                {
                    buffer_.decreaseSize(bytesReaded);
                }
                return Error::NoError;
            }

            return error;
        }

    private:
        class InternalInputBuffer final
            : public InputBuffer
        {
        public:
            explicit InternalInputBuffer(size_t size)
                : buffer_(size)
            {
                setBuffer(buffer_.ptr, size);
            }

            char* mutableData() const noexcept
            {
                return buffer_.ptr;
            }

            void decreaseSize(size_t newSize) noexcept
            {
                assert(newSize < maxSize_);
                maxSize_ = newSize;
            }

            void gotoBegin() noexcept
            {
                pos_ = 0;
            }

            void gotoEnd() noexcept
            {
                pos_ = maxSize_;
            }

            size_t available() const noexcept
            {
                return maxSize_ - pos_;
            }

        private:
            details::Buffer buffer_;
        };

        std::istream& in_;
        InternalInputBuffer buffer_;
        bool eof_;
    };

    class OutputStream final
    {
        static constexpr auto BufferSize = details::PrefferedBufferSize;
    public:
        explicit OutputStream(std::ostream& stream)
            : out_(stream)
            , buffer_(BufferSize)
        {
        }

        ~OutputStream()
        {
            flush();
        }

        OutputStream(const OutputStream&) = delete;
        OutputStream& operator=(const OutputStream&) = delete;

        OutputStream(OutputStream&&) = delete;
        OutputStream& operator=(OutputStream&&) = delete;

        template <class T>
        Error put(T value)
        {
            const auto error = buffer_.put(value);
            if (error == Error::NoError)
            {
                return Error::NoError;
            }

            if (error == Error::NotEnoughMemory)
            {
                PODS_SAFE_CALL(writeBuffer());
            }

            return buffer_.put(value);
        }

        Error put(const char* data, size_t size)
        {
            if (buffer_.available() >= size)
            {
                return buffer_.put(data, size);
            }

            PODS_SAFE_CALL(writeBuffer());

            const bool success = !!out_.write(data, static_cast<std::streamsize>(size));
            return success
                ? Error::NoError
                : Error::WriteError;
        }

        template <class T>
        Error put(const T* data, size_t size)
        {
            const auto totalSize = size * sizeof(T);
            return put(reinterpret_cast<const char*>(data), totalSize);
        }

        Error flush() noexcept
        {
            if (buffer_.size() > 0)
            {
                return writeBuffer();
            }
            return Error::NoError;
        }

    private:
        Error writeBuffer() noexcept
        {
            const bool success =
                !!out_.write(buffer_.data(), static_cast<std::streamsize>(buffer_.size()));
            if (success)
            {
                buffer_.clear();
                return Error::NoError;
            }
            return Error::WriteError;
        }

    private:
        std::ostream& out_;
        OutputBuffer buffer_;
    };
}
