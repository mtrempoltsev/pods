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
            , eof_(false)
        {
            buffer_.gotoEnd();
        }

        ReadOnlyStreamStorage(const ReadOnlyStreamStorage&) = delete;
        ReadOnlyStreamStorage& operator=(const ReadOnlyStreamStorage&) = delete;

        ReadOnlyStreamStorage(ReadOnlyStreamStorage&&) = delete;
        ReadOnlyStreamStorage& operator=(ReadOnlyStreamStorage&&) = delete;

        template <class T>
        Error get(T& value) noexcept
        {
            if (!eof_)
            {
                if (buffer_.available() >= sizeof(T))
                {
                    return buffer_.get(value);
                }

                const auto bytesLeft = buffer_.available();
                if (bytesLeft > 0)
                {
                    auto pos = in_.tellg();
                    pos -= bytesLeft;
                    in_.seekg(pos, std::ios_base::beg);
                }

                const auto error = readBuffer();
                if (error != Error::NoError)
                {
                    return error;
                }

                return buffer_.get(value);
            }

            return Error::UnexpectedEnd;
        }

        Error get(char* data, size_t size) noexcept
        {
            if (!eof_)
            {
                if (buffer_.available() >= size)
                {
                    return buffer_.get(data, size);
                }

                const auto bytesReaded = buffer_.available();
                const auto needToReed = size - bytesReaded;

                auto error = buffer_.get(data, bytesReaded);
                if (error != Error::NoError)
                {
                    return error;
                }

                error = read(data + bytesReaded, needToReed);
                if (error == Error::NoError || error == Error::Eof)
                {
                    return Error::NoError;
                }

                return error;
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
        Error read(char* data, size_t size) noexcept
        {
            const bool success = !!in_.read(data, size);
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

            const auto error = read(buffer_.mutableData(), PrefferedBufferSize);

            if (error == Error::Eof)
            {
                const auto bytesReaded = in_.gcount();
                if (bytesReaded < PrefferedBufferSize)
                {
                    buffer_.decreaseSize(bytesReaded);
                }
            }

            return error;
        }

    private:
        std::istream& in_;
        details::BufferedReadOnlyMemoryStorage buffer_;
        bool eof_;
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
