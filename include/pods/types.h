﻿#pragma once

#include <cstdint>
#include <limits>

namespace pods
{
    using Bool = uint8_t;
    using Size = uint32_t;
    using Version = uint32_t;

    static constexpr Bool False = 0;
    static constexpr Bool True = 1;
}