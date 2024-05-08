#include "CPU/DiscriptorTables.h"
/* R
	http://www.osdever.net/bkerndev/Docs/gdt.htm
	https://wiki.osdev.org/GDT_Tutorial
*/

gdt_entry	gdt_entries[GDT_ENTRIES];
gdt_ptr		gdt_p;

static void	setGdtEntry(uint32_t index, uint32_t limit, uint32_t base, uint8_t access_b, uint8_t flags){
	// define the size of the segment:
	gdt_entries[index].low_limit = limit & 0xFFFF;
	gdt_entries[index].flags_highLimit = (limit >> 16) & 0x0F;	// limit at the last for bits
	// define the base address of the segment:
	gdt_entries[index].low_base	= base & 0xFFFF;
	gdt_entries[index].middle_base = (base >> 16) & 0xFF;
	gdt_entries[index].high_base = (base >> 24) & 0xFF;
	// define the access/flag bits:
	gdt_entries[index].flags_highLimit |= flags & 0xF0;	// flags stored at the first 4 bits
	gdt_entries[index].access_bytes = access_b;
}

void	initGdt(){
	gdt_p.base = (uint32_t)&gdt_entries;
	gdt_p.limit = (GDT_ENTRIES * sizeof(gdt_entry)) - 1;

	setGdtEntry(0, 0, 0, 0, 0); // NULL Segment
	setGdtEntry(1, 0xFFFFFFFF, 0x0, 0x9A, 0xCF);	// Code Segment
	setGdtEntry(2, 0xFFFFFFFF, 0x0, 0x92, 0xCF);	// Data Segment
	
	// loadGdt((uint32_t)&gdt_p);
}