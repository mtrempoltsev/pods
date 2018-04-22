#pragma once

namespace pods
{
    namespace details
    {
#ifdef _MSC_VER
        inline int8_t changeEndianess(int8_t value) noexcept
        {
            return value;
        }

        inline uint8_t changeEndianess(uint8_t value) noexcept
        {
            return value;
        }

        template <class T, typename std::enable_if<sizeof(T) == 2, int>::type = 0>
        T changeEndianess(T value) noexcept
        {
            return _byteswap_ushort(value);
        }

        template <class T, typename std::enable_if<sizeof(T) == 4, int>::type = 0>
        T changeEndianess(T value) noexcept
        {
            return _byteswap_ulong(value);
        }

        template <class T, typename std::enable_if<sizeof(T) == 8, int>::type = 0>
        T changeEndianess(T value) noexcept
        {
            return _byteswap_uint64(value);
        }
#else
        inline int8_t changeEndianess(int8_t value) noexcept
        {
            return value;
        }

        inline uint8_t changeEndianess(uint8_t value) noexcept
        {
            return value;
        }

        template <class T, typename std::enable_if<sizeof(T) == 2, int>::type = 0>
        T changeEndianess(T value) noexcept
        {
            return __builtin_bswap16(value);
        }

        template <class T, typename std::enable_if<sizeof(T) == 4, int>::type = 0>
        T changeEndianess(T value) noexcept
        {
            return __builtin_bswap32(value);
        }

        template <class T, typename std::enable_if<sizeof(T) == 8, int>::type = 0>
        T changeEndianess(T value) noexcept
        {
            return __builtin_bswap64(value);
        }
#endif

#ifdef PODS_BIG_ENDIAN
        static constexpr bool BigEndian = true;
        static constexpr bool LittleEndian = false;

        template <class T>
        T toBigEndian(T value) noexcept
        {
            return value;
        }

        template <class T>
        T fromBigEndian(T value) noexcept
        {
            return value;
        }

        template <class T>
        T toLittleEndian(T value) noexcept
        {
            return changeEndianess(value);
        }

        template <class T>
        T fromLittleEndian(T value) noexcept
        {
            return changeEndianess(value);
        }
#endif

#ifdef PODS_LITTLE_ENDIAN
        static constexpr bool BigEndian = false;
        static constexpr bool LittleEndian = true;

        template <class T>
        T toBigEndian(T value) noexcept
        {
            return changeEndianess(value);
        }

        template <class T>
        T fromBigEndian(T value) noexcept
        {
            return changeEndianess(value);
        }

        template <class T>
        T toLittleEndian(T value) noexcept
        {
            return value;
        }

        template <class T>
        T fromLittleEndian(T value) noexcept
        {
            return value;
        }
#endif
    }
}