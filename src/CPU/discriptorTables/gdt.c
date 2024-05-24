#include "mem.h"
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
	gdtEntries[index].lowLimit = L16(limit);
	gdtEntries[index].highLimit = (limit >> 16) & 0x0F;	// limit at the last for bits
	// define the base address of the segment:
	gdtEntries[index].lowBase	= L16(base);
	gdtEntries[index].middleBase = (base >> 16) & 0xFF;
	gdtEntries[index].highBase = (base >> 24) & 0xFF;
	// define the access/flag bits:
	gdtEntries[index].flags = flags & 0xF;	// flags stored at the first 4 bits
	gdtEntries[index].accessByte = access_b;
}

void	initGdt(){

	setGdtEntry(0, 0, 0, 0, 0);						// NULL Segment
	setGdtEntry(1, 0xFFFFF, 0x0, 0x9A, 0x0C);		// Kernel Code Segment
	setGdtEntry(2, 0xFFFFF, 0x0, 0x92, 0x0C);		// Kernel Data Segment
	setGdtEntry(3, 0xFFFFF, 0x0, 0x92, 0x0C);		// Kernel Stack Segment

	setGdtEntry(4, 0xFFFFF, 0x0, 0xFA, 0x0C);		// User Code Segment
	setGdtEntry(5, 0xFFFFF, 0x0, 0xF2, 0x0C);		// User Data Segment
	setGdtEntry(6, 0xFFFFF, 0x0, 0xF2, 0x0C);		// User Stack Segment

	memcpy((void *)GDT_BASEADDR, gdtEntries, (GDT_ENTRIES * sizeof(gdtEntry)) - 1);
	gdtP.base = GDT_BASEADDR;
	// gdtP.base = (uint32_t)&gdtEntries;
	gdtP.limit = (GDT_ENTRIES * sizeof(gdtEntry)) - 1;
	loadGdt((uint32_t)&gdtP);
}