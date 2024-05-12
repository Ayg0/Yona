#pragma once

#include "types.h"

typedef struct _varg_ptr
{
	uint32_t *ptr;
} varg_ptr;

#define VARG_INIT(V_PTR, FIRST_ARG) V_PTR.ptr = (uint32_t *)&FIRST_ARG;
#define VARG_NEXT(V_PTR, TYPE) (*(TYPE *)(++(V_PTR).ptr))