#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace pods
{
    struct Binary
    {
        Binary(uint8_t* buffer, size_t bufferSize)
            : data(buffer)
            , size(bufferSize)
        {
        }

        template <class T, size_t ArraySize>
        Binary(std::array<T, ArraySize>& value)
            : data(reinterpret_cast<uint8_t*>(value.data()))
            , size(value.size() * sizeof(T))
        {
        }

        template <class T>
        Binary(std::vector<T>& value)
            : data(reinterpret_cast<uint8_t*>(value.data()))
            , size(value.size() * sizeof(T))
        {
        }

        ~Binary() = default;

        Binary(Binary&) = default;
        Binary& operator=(const Binary&) = default;

        Binary(Binary&&) = default;
        Binary& operator=(Binary&&) = default;

        uint8_t* data;
        size_t size;
    };
}
