#pragma once

#ifdef PODS_MDR
#error Rename the macro
#endif
#define PODS_MDR(field) #field, field

#ifdef PODS_OPT
#error Rename the macro
#endif
#define PODS_OPT(field) #field, field

#ifdef PODS_SERIALIZABLE
#error Rename the macro
#endif
#define PODS_SERIALIZABLE(dataVersion, ...)                             \
    static constexpr pods::Version version() { return dataVersion; }    \
                                                                        \
    template <class Serializer>                                         \
    pods::Error serialize(Serializer& serializer, pods::Version)        \
    {                                                                   \
        return serializer(__VA_ARGS__);                                 \
    }                                                                   \

