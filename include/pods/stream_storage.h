#pragma once

#include <istream>
#include <ostream>

#include "details/settings.h"
#include "details/utils.h"

#include "memory_storage.h"

namespace pods
{
    class ReadOnlyStreamStorage final
    {
        static constexpr auto BufferSize = details::PrefferedBufferSize;
    public:
        explicit ReadOnlyStreamStorage(std::istream& stream)
            : in_(stream)
            , buffer_(BufferSize)
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

            if (eof_)
            {
                return Error::UnexpectedEnd;
            }

            PODS_SAFE_CALL(readBuffer());

            return buffer_.get(value);
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

                PODS_SAFE_CALL(buffer_.get(data, bytesReaded));

                const auto error = read(data + bytesReaded, needToReed);
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
        std::istream& in_;
        details::BufferedReadOnlyMemoryStorage buffer_;
        bool eof_;
    };

    class WriteOnlyStreamStorage final
    {
        static constexpr auto BufferSize = details::PrefferedBufferSize;
    public:
        explicit WriteOnlyStreamStorage(std::ostream& stream)
            : out_(stream)
            , buffer_(BufferSize)
        {
        }

        ~WriteOnlyStreamStorage()
        {
            flush();
        }

        WriteOnlyStreamStorage(const WriteOnlyStreamStorage&) = delete;
        WriteOnlyStreamStorage& operator=(const WriteOnlyStreamStorage&) = delete;

        WriteOnlyStreamStorage(WriteOnlyStreamStorage&&) = delete;
        WriteOnlyStreamStorage& operator=(WriteOnlyStreamStorage&&) = delete;

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
