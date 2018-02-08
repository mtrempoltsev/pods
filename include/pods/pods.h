#pragma once

#include "binary.h"
#include "errors.h"
#include "types.h"

#ifdef PODS_MDR
#error Rename the macro
#endif
#define PODS_MDR(field) #field, field

#ifdef PODS_MDR_BIN
#error Rename the macro
#endif
#define PODS_MDR_BIN(field) #field, ::pods::Binary(field)

#ifdef PODS_OPT
#error Rename the macro
#endif
#define PODS_OPT(field) "\0" #field, field

#ifdef PODS_OPT_BIN
#error Rename the macro
#endif
#define PODS_OPT_BIN(field) "\0" #field, ::pods::Binary(field)

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

