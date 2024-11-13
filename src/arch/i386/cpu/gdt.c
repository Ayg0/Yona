#include "klibc/types.h"
#include "klibc/memory.h"
#include "arch/i386/idts.h"

_gdtEntry	gdtEntries[GDT_ENTRIES];
_gdtPtr		gdtPtr;

extern void	gdtLoader(_gdtPtr *gdtPtr);

void	setGdtEntry(uint8_t index, uint32_t limit, uint32_t base, uint8_t accessByte, uint8_t flags){
	// define the size of the segment:
	gdtEntries[index].lowLimit = L16(limit);
	gdtEntries[index].highLimit = (limit >> 16) & 0x0F;	// limit at the last for bits
	// define the base address of the segment:
	gdtEntries[index].lowBase	= L16(base);
	gdtEntries[index].middleBase = (base >> 16) & 0xFF;
	gdtEntries[index].highBase = (base >> 24) & 0xFF;
	// define the access/flag bits:
	gdtEntries[index].flags = flags & 0xF;	// flags stored at the first 4 bits
	gdtEntries[index].accessByte = accessByte;
}

void	initGdt(){
	setGdtEntry(0, 0, 0, 0, 0);						// NULL Segment

	setGdtEntry(1, 0xFFFFF, 0x0, 0x9A, 0x0C);		// Kernel Code Segment
	setGdtEntry(2, 0xFFFFF, 0x0, 0x92, 0x0C);		// Kernel Data Segment
	setGdtEntry(3, 0xFFFFF, 0x0, 0x92, 0x0C);		// Kernel Stack Segment

	setGdtEntry(4, 0xFFFFF, 0x0, 0xFA, 0x0C);		// User Code Segment
	setGdtEntry(5, 0xFFFFF, 0x0, 0xF2, 0x0C);		// User Data Segment
	setGdtEntry(6, 0xFFFFF, 0x0, 0xF2, 0x0C);		// User Stack Segment

	memcpy((void *)GDT_BASEADDR, gdtEntries, (GDT_ENTRIES * sizeof(_gdtEntry)) - 1);
	gdtPtr.base = GDT_BASEADDR;
	// gdtPtr.base = (uint32_t)&gdtEntries;
	gdtPtr.limit = (GDT_ENTRIES * sizeof(_gdtEntry)) - 1;
	gdtLoader(&gdtPtr);
}