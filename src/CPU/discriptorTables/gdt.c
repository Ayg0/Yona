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
	gdtEntries[index].flags |= 0xf & flags;	// flags stored at the first 4 bits
	gdtEntries[index].accessByte = access_b;
}

// init_gdt_desc(0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);    /* ucode */
// init_gdt_desc(0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);    /* udata */
// init_gdt_desc(0x0, 0x0,	0xF7,	0x0D, &kgdt[6]);        /* ustack */

void	initGdt(){

	setGdtEntry(0, 0, 0, 0, 0);						// NULL Segment
	setGdtEntry(1, 0xFFFFF, 0x0, 0x9A, 0xC);		// Kernel Code Segment
	setGdtEntry(2, 0xFFFFF, 0x0, 0x92, 0xC);		// Kernel Data Segment

	setGdtEntry(3, 0xFFFFF, 0x0, 0xFA, 0xC);		// User Code Segment
	setGdtEntry(4, 0xFFFFF, 0x0, 0xF2, 0xC);		// User Data Segment

	memcpy(GDT_BASEADDR, gdtEntries, (GDT_ENTRIES * sizeof(gdtEntry)) - 1);
	gdtP.base = GDT_BASEADDR;
	gdtP.limit = (GDT_ENTRIES * sizeof(gdtEntry)) - 1;
	loadGdt((uint32_t)&gdtP);
}