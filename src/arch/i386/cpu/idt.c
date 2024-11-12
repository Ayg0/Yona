#include "arch/i386/idts.h"
#include "klibc/print.h"
#include "klibc/memory.h"

__attribute__((aligned(0x10)))
_idtEntry idt[256];
/*
	When the processor fetches data from memory, it does so in chunks that align 
	with the memory’s physical architecture.If data structures are aligned with these boundaries,
	the processor can fetch them in fewer cycles,
	which can lead to more efficient execution.
*/
_idtPtr     idtPtr;

void (*handlers[USED_HANDLERS])() = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9,
    isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19,
    isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29,
    isr30, isr31,
    irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9,
    irq10, irq11, irq12, irq13, irq14, irq15
};

void	initIdtEntry(uint8_t index, void *isr, uint8_t attr){
	_idtEntry *entry = &idt[index];

	entry->always0 = 0;
	entry->attributes = attr;
	entry->kernelCS = 0x08;	// code segment
	entry->isrLow = (uint32_t)isr & 0xffff;
	entry->isrHigh = (uint32_t)isr >> 16;

}

void	isrHandler(_registers Rs){
	if (Rs.intNbr < 32){
        PRINT_K("\r\n[Fatal Error] Exception Raised => Nbr: %d, ErrCode: %d\r\n", Rs.intNbr, Rs.errCode);
        S_ERR("Exception Raised => Nbr: %d, ErrCode: %d\r\n", Rs.intNbr, Rs.errCode);
		__asm__ __volatile__ ("cli; hlt");
	}
}

void	initIdtEntries(){
	for (uint8_t i = 0; i < USED_HANDLERS; i++)
		initIdtEntry(i, handlers[i], 0x8E);
}

void	initIdt(){
	idtPtr.base = (uint32_t)&idt;
	idtPtr.limit = sizeof(_idtEntry) * 255; // * 256 - 1
	bzero(idt, sizeof(_idtEntry) * 255);
	picRemap(32, 40);
	initIdtEntries();

	__asm__ __volatile__ ("lidt (%%eax)": : "a"(&idtPtr));	// loading the idt into the IDTR.
	__asm__ __volatile__ ("sti");
}
