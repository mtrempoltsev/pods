#pragma once

namespace spp
{
    template <class T>
    struct MandatoryNameValue
    {
        const char* name_;
        T& value_;
    };

    template <class T>
    struct OptionalNameValue
    {
        const char* name_;
        T& value_;
    };
}

#define SPP_MDR(field) #field, field
#define SPP_OPT(field) #field, field
