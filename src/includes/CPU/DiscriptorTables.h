#pragma once
#include "types.h"

# define GDT_ENTRIES 3

// each gdt_entry
typedef	struct _gdt_entry{
	uint16_t	low_limit;		// 0-15 bits of the limit
	uint16_t	low_base;		// 0-15 bits of the base
	uint8_t	middle_base;		// 16-23 bits of the base
	uint8_t	access_bytes;		// access bytes
	uint8_t	flags_highLimit;	// flags and the last 4 bits of the limit
	uint8_t	high_base;			// 24-31 bits of the base
}__attribute__ ((packed)) gdt_entry; // __attribute__((packed)) provent any padding.
// the GDT_descriptor
typedef	struct	_gdt_p{
	uint16_t	limit;	// size of the GDT
	uint32_t	base;	//	start of the GDT
}__attribute__ ((packed)) gdt_ptr;
