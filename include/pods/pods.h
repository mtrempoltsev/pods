#pragma once

#include "binary_serializer.h"

#ifdef PODS_MDR
#error Rename the macro
#endif
#define PODS_MDR(field) #field, field

#ifdef PODS_OPT
#error Rename the macro
#endif
#define PODS_OPT(field) #field, field
