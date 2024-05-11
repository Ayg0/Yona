#pragma once

typedef struct VARG_PTR
{
	uint32_t *ptr;
} VARG_PTR;

#define VARG_INIT(V_PTR, FIRST_ARG) V_PTR.ptr = (uint32_t *)&FIRST_ARG;
#define VARG_NEXT(V_PTR, TYPE) (*(TYPE *)(++(V_PTR).ptr))