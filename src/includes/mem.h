#pragma once
#include "types.h"

# define CLEAR_MEM(TYPE, MEM, NB) {TYPE *m = (TYPE *) MEM;\
									for (size_t i = 0; i < NB; i++)\
										m[i] = 0;}

void *memcpy(void *dest, const void *src, size_t n);