#include "CPU/DiscriptorTables.h"
/* R
	http://www.osdever.net/bkerndev/Docs/gdt.htm
	https://wiki.osdev.org/GDT_Tutorial
	https://www.youtube.com/watch?v=Wh5nPn2U_1w
*/

gdtEntry	gdtEntries[GDT_ENTRIES];
gdtPtr		gdtP;

static void	setGdtEntry(uint32_t index, uint32_t limit, uint32_t base, uint8_t access_b, uint8_t flags){
	// define the size of the segment:
	gdtEntries[index].lowLimit = limit & 0xFFFF;
	gdtEntries[index].flags_highLimit = (limit >> 16) & 0x0F;	// limit at the last for bits
	// define the base address of the segment:
	gdtEntries[index].lowBase	= base & 0xFFFF;
	gdtEntries[index].middleBase = (base >> 16) & 0xFF;
	gdtEntries[index].highBase = (base >> 24) & 0xFF;
	// define the access/flag bits:
	gdtEntries[index].flags_highLimit |= flags & 0xF0;	// flags stored at the first 4 bits
	gdtEntries[index].accessByte = access_b;
}

void	initGdt(){
	gdtP.base = (uint32_t)&gdtEntries;
	gdtP.limit = (GDT_ENTRIES * sizeof(gdtEntry)) - 1;

	setGdtEntry(0, 0, 0, 0, 0);						// NULL Segment
	setGdtEntry(1, 0xFFFFFFFF, 0x0, 0x9A, 0xCF);	// Kernel Code Segment
	setGdtEntry(2, 0xFFFFFFFF, 0x0, 0x92, 0xCF);	// Kernel Data Segment
	
	loadGdt((uint32_t)&gdtP);
}