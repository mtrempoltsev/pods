#pragma once

#include <cstdint>
#include <limits>

namespace pods
{
    using Bool = uint8_t;
    using Size = uint32_t;
    using Version = uint32_t;

    static constexpr Bool False = 0;
    static constexpr Bool True = 1;

    static constexpr Version NoVersion = 0;

    static constexpr Size MaxSize = std::numeric_limits<Size>::max() - 1;
}
