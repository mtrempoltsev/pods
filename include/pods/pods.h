#pragma once

#include "errors.h"
#include "types.h"

#include "details/binary_wrappers.h"

#ifdef PODS_MDR
#error Rename the macro
#endif
#define PODS_MDR(field) #field, field

#ifdef PODS_MDR_BIN
#error Rename the macro
#endif
#define PODS_MDR_BIN(field) #field, ::pods::details::makeBinary(field)

#ifdef PODS_MDR_BIN_2
#error Rename the macro
#endif
#define PODS_MDR_BIN_2(field, size) #field, ::pods::details::makeBinary2(field, size)

#ifdef PODS_OPT
#error Rename the macro
#endif
#define PODS_OPT(field) "\0" #field, field

#ifdef PODS_OPT_BIN
#error Rename the macro
#endif
#define PODS_OPT_BIN(field) "\0" #field, ::pods::details::makeBinary(field)

#ifdef PODS_OPT_BIN_2
#error Rename the macro
#endif
#define PODS_OPT_BIN_2(field, size) "\0" #field, ::pods::details::makeBinary2(field, size)

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

