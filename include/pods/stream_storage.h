#pragma once

#include <istream>
#include <ostream>

#include "memory_storage.h"

namespace pods
{
    class ReadOnlyStreamStorage final
    {
    public:
        explicit ReadOnlyStreamStorage(std::istream& stream)
            : in_(stream)
            , buffer_(PrefferedBufferSize)
        {
        }

        ReadOnlyStreamStorage(const ReadOnlyStreamStorage&) = delete;
        ReadOnlyStreamStorage& operator=(const ReadOnlyStreamStorage&) = delete;

        ReadOnlyStreamStorage(ReadOnlyStreamStorage&&) = delete;
        ReadOnlyStreamStorage& operator=(ReadOnlyStreamStorage&&) = delete;

        template <class T>
        Error get(T& value) noexcept
        {
            if (in_.read(reinterpret_cast<char*>(&value), sizeof(T)))
            {
                return Error::NoError;
            }

            return in_.eof()
                ? Error::NoError
                : Error::ReadError;
        }

        Error get(char* data, size_t size) noexcept
        {
            if (in_.read(data, size))
            {
                return Error::NoError;
            }

            return in_.eof()
                ? Error::NoError
                : Error::ReadError;
        }

        template <class T>
        Error get(T* data, size_t size) noexcept
        {
            const auto totalSize = size * sizeof(T);
            return get(reinterpret_cast<char*>(data), totalSize);
        }

    private:
        std::istream& in_;
        FixedSizeWriteOnlyMemoryStorage buffer_;
    };

    class WriteOnlyStreamStorage final
    {
    public:
        explicit WriteOnlyStreamStorage(std::ostream& stream)
            : out_(stream)
            , buffer_(PrefferedBufferSize)
        {
        }

        WriteOnlyStreamStorage(const WriteOnlyStreamStorage&) = delete;
        WriteOnlyStreamStorage& operator=(const WriteOnlyStreamStorage&) = delete;

        WriteOnlyStreamStorage(WriteOnlyStreamStorage&&) = delete;
        WriteOnlyStreamStorage& operator=(WriteOnlyStreamStorage&&) = delete;

        template <class T>
        Error put(T value)
        {
            auto error = buffer_.put(value);
            if (error == Error::NoError)
            {
                return Error::NoError;
            }

            if (error == Error::NotEnoughMemory)
            {
                error = writeBuffer();
                if (error != Error::NoError)
                {
                    return error;
                }
            }

            return buffer_.put(value);
        }

        Error put(const char* data, size_t size)
        {
            if (buffer_.available() >= size)
            {
                return buffer_.put(data, size);
            }

            auto error = writeBuffer();
            if (error != Error::NoError)
            {
                return error;
            }

            const bool success = !!out_.write(data, size);
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

    private:
        Error writeBuffer() noexcept
        {
            const bool success = !!out_.write(buffer_.data(), buffer_.size());
            if (success)
            {
                buffer_.clear();
                return Error::NoError;
            }
            return Error::WriteError;
        }

    private:
        std::ostream& out_;
        FixedSizeWriteOnlyMemoryStorage buffer_;
    };
}
