#pragma once
#include "types.h"

/*--------------------------- GDT ---------------------------*/
# define GDT_ENTRIES 7
# define GDT_BASEADDR 0x00000800

// gdt entry structure:
typedef	struct gdtEntry{
	uint16_t	lowLimit;			// 0-15 bits of the limit
	uint16_t	lowBase;			// 0-15 bits of the base
	uint8_t		middleBase;		// 16-23 bits of the base
	/*
		|7|6 5|4|3|2 |1 |0|
		|P|DPL|S|E|DC|RW|A|
		Access Byte: learn more https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor
	*/
	uint8_t		accessByte;		// access bytes
	/*
		|3|2 |1|0       |
		|G|DB|L|Reserved|
		*G* -> when this set to 1 the limit got multiplayed by 0x1000 (to get 4Gb)
	*/
	uint8_t	flags:4;
	uint8_t highLimit:4;
	uint8_t	highBase;			// 24-31 bits of the base
}__attribute__ ((packed)) _gdtEntry; // __attribute__((packed)) provent any padding.

typedef	struct	gdtPtr{
	uint16_t	limit;	// size of the GDT
	uint32_t	base;	//	start of the GDT
}__attribute__ ((packed)) _gdtPtr;

void	initGdt();
