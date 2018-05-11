#pragma once

#include <cstdint>

namespace pods
{
    namespace details
    {
        namespace msgpack
        {
            using Tag = uint8_t;

            static constexpr Tag True = 0xc3;
            static constexpr Tag False = 0xc2;

            static constexpr Tag Int8 = 0xd0;
            static constexpr Tag UInt8 = 0xcc;

            static constexpr Tag Int16 = 0xd1;
            static constexpr Tag UInt16 = 0xcd;

            static constexpr Tag Int32 = 0xd2;
            static constexpr Tag UInt32 = 0xce;

            static constexpr Tag Int64 = 0xd3;
            static constexpr Tag UInt64 = 0xcf;

            static constexpr Tag Float = 0xca;
            static constexpr Tag Double = 0xcb;

            static constexpr Tag StrMax = 31;
            static constexpr Tag StrMask = 0b10100000;
            static constexpr Tag StrValue = 0b00011111;

            static constexpr Tag Str8 = 0xd9;
            static constexpr Tag Str16 = 0xda;
            static constexpr Tag Str32 = 0xdb;

            static constexpr Tag Bin8 = 0xc4;
            static constexpr Tag Bin16 = 0xc5;
            static constexpr Tag Bin32 = 0xc6;

            static constexpr Tag ArrayMax = 15;
            static constexpr Tag ArrayMask = 0b10010000;
            static constexpr Tag ArrayValue = 0b00001111;

            static constexpr Tag Array16 = 0xdc;
            static constexpr Tag Array32 = 0xdd;

            static constexpr Tag MapMax = 15;
            static constexpr Tag MapMask = 0b10000000;
            static constexpr Tag MapValue = 0b00001111;

            static constexpr Tag Map16 = 0xde;
            static constexpr Tag Map32 = 0xdf;

            static constexpr int8_t Min5 = -32;
            static constexpr int8_t Min8 = std::numeric_limits<int8_t>::min();
            static constexpr int8_t Max8 = std::numeric_limits<int8_t>::max();
            static constexpr int16_t Min16 = std::numeric_limits<int16_t>::min();
            static constexpr int16_t Max16 = std::numeric_limits<int16_t>::max();
            static constexpr int32_t Min32 = std::numeric_limits<int32_t>::min();
            static constexpr int32_t Max32 = std::numeric_limits<int32_t>::max();
            static constexpr int64_t Min64 = std::numeric_limits<int64_t>::min();
            static constexpr int64_t Max64 = std::numeric_limits<int64_t>::max();

            static constexpr uint8_t Max7U = 127u;
            static constexpr uint8_t Max8U = std::numeric_limits<uint8_t>::max();
            static constexpr uint16_t Max16U = std::numeric_limits<uint16_t>::max();
            static constexpr uint32_t Max32U = std::numeric_limits<uint32_t>::max();
            static constexpr uint64_t Max64U = std::numeric_limits<uint64_t>::max();
        }
    }
}
