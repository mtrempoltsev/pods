#pragma once

namespace pods
{
    namespace details
    {
        struct JsonTraits
        {
            static constexpr bool OnlyStringKeys = true;
            static constexpr bool OnlyUniqueKeys = true;
            static constexpr bool SupportsSeqSizes = false;
        };

        struct MsgPackTraits
        {
            static constexpr bool OnlyStringKeys = false;
            static constexpr bool OnlyUniqueKeys = false;
            static constexpr bool SupportsSeqSizes = true;
        };
    }
}
