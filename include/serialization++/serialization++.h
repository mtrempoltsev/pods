#pragma once

#include "binary_serializer.h"

#ifdef SPP_MDR
#error Rename the macro
#endif
#define SPP_MDR(field) #field, field

#ifdef SPP_OPT
#error Rename the macro
#endif
#define SPP_OPT(field) #field, field
