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
    public:
        explicit InputStream(std::istream& stream, size_t bufferSize = details::PrefferedBufferSize)
            : in_(stream)
            , buffer_(bufferSize)
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
            const size_t available = buffer_.available();
            if (available >= sizeof(T))
            {
                return buffer_.get(value);
            }

            if (in_.eof())
            {
                return Error::UnexpectedEnd;
            }

            if (available > 0)
            {
                auto pos = in_.tellg();
                pos -= static_cast<std::streamsize>(available);
                in_.seekg(pos, std::ios_base::beg);
            }

            PODS_SAFE_CALL(readBuffer());
            return buffer_.get(value);
        }

        Error get(char* data, size_t size)
        {
            const size_t available = buffer_.available();
            if (available >= size)
            {
                return buffer_.get(data, size);
            }

            if (in_.eof())
            {
                return Error::UnexpectedEnd;
            }

            if (available > 0)
            {
                auto pos = in_.tellg();
                pos -= static_cast<std::streamsize>(available);
                in_.seekg(pos, std::ios_base::beg);
            }

            if (size > buffer_.maxSize())
            {
                PODS_SAFE_CALL(read(data, size));
                buffer_.gotoEnd();
                return Error::NoError;
            }

            PODS_SAFE_CALL(readBuffer());
            return buffer_.get(data, size);
        }

        template <class T>
        Error get(T* data, size_t size)
        {
            const auto totalSize = size * sizeof(T);
            return get(reinterpret_cast<char*>(data), totalSize);
        }

    private:
        Error readBuffer()
        {
            PODS_SAFE_CALL(read(buffer_.data(), buffer_.maxSize()));
            buffer_.reset(static_cast<size_t>(in_.gcount()));
            return Error::NoError;
        }

        Error read(char* data, size_t size)
        {
            const bool success = !!in_.read(data, static_cast<std::streamsize>(size));
            return success || in_.eof()
                ? Error::NoError
                : Error::ReadError;
        }

    private:
        std::istream& in_;
        InputBuffer buffer_;
    };

    class OutputStream final
    {
    public:
        explicit OutputStream(std::ostream& stream, size_t bufferSize = details::PrefferedBufferSize)
            : out_(stream)
            , buffer_(bufferSize)
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

            return buffer_.size() > size
                ? buffer_.put(data, size)
                : write(data, size);
        }

        template <class T>
        Error put(const T* data, size_t size)
        {
            const auto totalSize = size * sizeof(T);
            return put(reinterpret_cast<const char*>(data), totalSize);
        }

        Error flush()
        {
            if (buffer_.size() > 0)
            {
                return writeBuffer();
            }
            return Error::NoError;
        }

    private:
        Error writeBuffer()
        {
            PODS_SAFE_CALL(write(buffer_.data(), buffer_.size()));
            buffer_.clear();
            return Error::NoError;
        }

        Error write(const char* data, size_t size)
        {
            const bool success = !!out_.write(data, static_cast<std::streamsize>(size));
            return success
                ? Error::NoError
                : Error::WriteError;
        }

    private:
        std::ostream& out_;
        OutputBuffer buffer_;
    };
}
