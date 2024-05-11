#pragma once
#include "types.h"

# define GDT_ENTRIES 3

/*--------------------------- GDT ---------------------------*/
// each gdt_entry
typedef	struct _gdtEntry{
	uint16_t	lowLimit;			// 0-15 bits of the limit
	uint16_t	lowBase;			// 0-15 bits of the base
	uint8_t		middleBase;		// 16-23 bits of the base
	uint8_t		accessByte;		// access bytes
	/*
		|7|6 5|4|3|2 |1 |0|
		|P|DPL|S|E|DC|RW|A|
		Access Byte: learn more https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor
	*/
	uint8_t	flags_highLimit;	// flags and the last 4 bits of the limit
	/*
		|3|2 |1|0       |
		|G|DB|L|Reserved|
		*G* -> when this set to 1 the limit got multiplayed by 0x1000 (to get 4Gb)
	*/
	uint8_t	highBase;			// 24-31 bits of the base
}__attribute__ ((packed)) gdtEntry; // __attribute__((packed)) provent any padding.
// the GDT_descriptor
typedef	struct	_gdtPtr{
	uint16_t	limit;	// size of the GDT
	uint32_t	base;	//	start of the GDT
}__attribute__ ((packed)) gdtPtr;
extern void	loadGdt();

/*--------------------------- IDT ---------------------------*/

#define USED_HANDLERS 48

typedef struct _idtEntry{
	uint16_t	isrLow;	// The lower 16 bits of the ISR's address
	uint16_t	kernelCS;	// The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t		always0;	// Set to zero
	uint8_t		attributes;	// Type and attributes
	uint16_t	isrHigh;	// The higher 16 bits of the ISR's address
} __attribute__((packed)) idtEntry;

typedef struct _idtPtr{
	uint16_t	limit;
	uint32_t	base;		// address of the first element in the idtEntries array
}__attribute__((packed)) idtPtr;

typedef struct _registers{
	// R https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html
	uint32_t	ds;	// data segment selector
	uint32_t	edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha R https://pdos.csail.mit.edu/6.828/2005/readings/i386/PUSHA.htm
	uint32_t	intNbr, errCode;	// interrupt index and error code
	uint32_t	eip, cs, eflags, useresp, ss; // pushed by the cpu automatically
} registers;

void		initDTs();

// CPU exceptions ISRs
extern	void	isr0();
extern	void	isr1();
extern	void	isr2();
extern	void	isr3();
extern	void	isr4();
extern	void	isr5();
extern	void	isr6();
extern	void	isr7();
extern	void	isr8();
extern	void	isr9();
extern	void	isr10();
extern	void	isr11();
extern	void	isr12();
extern	void	isr13();
extern	void	isr14();
extern	void	isr15();
extern	void	isr16();
extern	void	isr17();
extern	void	isr18();
extern	void	isr19();
extern	void	isr20();
extern	void	isr21();
extern	void	isr22();
extern	void	isr23();
extern	void	isr24();
extern	void	isr25();
extern	void	isr26();
extern	void	isr27();
extern	void	isr28();
extern	void	isr29();
extern	void	isr30();
extern	void	isr31();
// IRQs
extern	void	irq0();
extern	void	irq1();
extern	void	irq2();
extern	void	irq3();
extern	void	irq4();
extern	void	irq5();
extern	void	irq6();
extern	void	irq7();
extern	void	irq8();
extern	void	irq9();
extern	void	irq10();
extern	void	irq11();
extern	void	irq12();
extern	void	irq13();
extern	void	irq14();
extern	void	irq15();

void	clearIRQHandler(uint8_t irq);
void	setIRQHandler(uint8_t irq, void (*handler)(registers r));