#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "../errors.h"

namespace pods
{
    namespace details
    {
        struct BinaryArray final
        {
        public:
            BinaryArray(char* data, size_t size) noexcept
                : data_(data)
                , size_(size)
            {
            }

            Error allocate(size_t requestedSize)
            {
                return requestedSize == size_
                    ? Error::NoError
                    : Error::CorruptedArchive;
            }

            char* data() const noexcept
            {
                return data_;
            }
            
            size_t size() const noexcept
            {
                return size_;
            }

        private:
            char* data_;
            size_t size_;
        };

        template <class T>
        struct BinaryVector final
        {
        public:
            static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

            explicit BinaryVector(std::vector<T>& storage)
                : storage_(storage)
            {
            }

            Error allocate(char*& ptr, size_t requestedSize)
            {
                storage_.resize(requestedSize / sizeof(T));
                ptr = data();
                return Error::NoError;
            }

            char* data() const noexcept
            {
                return reinterpret_cast<char*>(storage_.data());
            }

            size_t size() const noexcept
            {
                return storage_.size() * sizeof(T);
            }

        private:
            std::vector<T>& storage_;
        };

        template <class T, size_t ArraySize>
        BinaryArray makeBinary(T (&value)[ArraySize])
        {
            return BinaryArray(reinterpret_cast<char*>(value), ArraySize * sizeof(T));
        }

        template <class T, size_t ArraySize>
        BinaryArray makeBinary(std::array<T, ArraySize>& value)
        {
            return BinaryArray(reinterpret_cast<char*>(value.data()), value.size() * sizeof(T));
        }

        template <class T>
        BinaryVector<T> makeBinary(std::vector<T>& value)
        {
            return BinaryVector<T>(value);
        }

        template <class T>
        BinaryArray makeBinary2(const std::unique_ptr<T[]>& data, size_t size)
        {
            return BinaryArray(reinterpret_cast<char*>(data.get()), size * sizeof(T));
        }

        template <class T>
        BinaryArray makeBinary2(T* data, size_t size)
        {
            return BinaryArray(reinterpret_cast<char*>(data), size * sizeof(T));
        }
    }
}
