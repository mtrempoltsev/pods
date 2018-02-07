#pragma once

#include <string>

namespace pods
{
    namespace details
    {
        size_t getBase64EncodedSize(size_t size)
        {
            const auto result = 4 * ((size + 2) / 3);
            return result < size
                ? 0 // integer overflow
                : result;
        }

        std::string base64Encode(const char* source, size_t size)
        {
            static constexpr char Base64Chars[65] =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

            const auto resultSize = getBase64EncodedSize(size);

            std::string result;
            result.resize(resultSize);

            auto in = reinterpret_cast<const uint8_t*>(source);
            auto end = in + size;

            auto out = const_cast<char*>(result.c_str());

            while (end - in >= 3)
            {
                *out++ = Base64Chars[in[0] >> 2];
                *out++ = Base64Chars[((in[0] & 0x03) << 4) | (in[1] >> 4)];
                *out++ = Base64Chars[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
                *out++ = Base64Chars[in[2] & 0x3f];

                in += 3;
            }

            if (end - in > 0)
            {
                *out++ = Base64Chars[in[0] >> 2];

                if (end - in == 1)
                {
                    *out++ = Base64Chars[(in[0] & 0x03) << 4];
                    *out++ = '=';
                }
                else
                {
                    *out++ = Base64Chars[((in[0] & 0x03) << 4) |
                        (in[1] >> 4)];
                    *out++ = Base64Chars[(in[1] & 0x0f) << 2];
                }

                *out++ = '=';
            }

            return result;
        }

        size_t getBase64DecodedSize(const char* encoded, size_t size)
        {
            const auto padding = size < 4
                ? 0
                : encoded[size - 2] == '='
                    ? 2
                    : encoded[size - 1] == '='
                        ? 1
                        : 0;
            return (size * 3) / 4 - padding;
        }

        void base64Decode(const char* encoded, size_t size, char* destination)
        {
            assert(size % 4 == 0);

            if (size == 0)
            {
                return;
            }

            static constexpr uint32_t Base64Index[256] =
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 62, 62, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,
                0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                22, 23, 24, 25, 0, 0, 0, 0, 63, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
            };

            const bool hasPadding = (encoded[size - 1] == '=');
            const auto end = encoded + size;

            for (size_t i = hasPadding ? 1 : 0; i < size / 4; ++i)
            {
                uint32_t n = Base64Index[*encoded++] << 18;
                n |= Base64Index[*encoded++] << 12;
                n |= Base64Index[*encoded++] << 6;
                n |= Base64Index[*encoded++];

                *destination++ = static_cast<char>(n >> 16);
                *destination++ = static_cast<char>(n >> 8 & 0xFF);
                *destination++ = static_cast<char>(n & 0xFF);
            }

            if (encoded != end)
            {
                uint32_t n = Base64Index[*encoded++] << 18;
                n |= Base64Index[*encoded++] << 12;

                *destination++ = static_cast<char>(n >> 16);

                if (encoded != end && *encoded != '=')
                {
                    n |= Base64Index[*encoded++] << 6;

                    *destination++ = static_cast<char>(n >> 8 & 0xFF);
                }
            }
        }
    }
}
