#include "CPU/DiscriptorTables.h"
#include "serialio.h"
#include "mem.h"
#include "tty.h"
/* R
	https://web.archive.org/web/20021020082044/http://www.mega-tokyo.com/os/os-faq-pics.html
	Read all 3 parts: http://www.osdever.net/tutorials/view/interrupts-exceptions-and-idts-part-1-interrupts-isrs-irqs-the-pic
	http://www.osdever.net/bkerndev/Docs/idt.htm <<<<<===== the best
	https://wiki.osdev.org/Interrupts_tutorial
*/
void picRemap(uint8_t masterOffset, uint8_t slaveOffset);

__attribute__((aligned(0x10)))
idtEntry idtEntries[256];
/*
	When the processor fetches data from memory, it does so in chunks that align 
	with the memory’s physical architecture.If data structures are aligned with these boundaries,
	the processor can fetch them in fewer cycles,
	which can lead to more efficient execution.
*/
void (*handlers[USED_HANDLERS])() = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9,
    isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19,
    isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29,
    isr30, isr31,
    irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9,
    irq10, irq11, irq12, irq13, irq14, irq15
};
idtPtr	idtP;

void	initIdtEntry(uint8_t index, void *isr, uint8_t attr){
	idtEntry *entry = &idtEntries[index];

	entry->always0 = 0;
	entry->attributes = attr;
	entry->kernelCS = 0x08;	// code segment
	entry->isrLow = (uint32_t)isr & 0xffff;
	entry->isrHigh = (uint32_t)isr >> 16;

}

void	isrHandler(registers Rs){
	if (Rs.intNbr < 32){
		changeTtyColor(VGA_RED, -1);
		serialPutStr("[Fatal Error] Exception Raised => Nbr: ");
		serialPutNbr(Rs.intNbr, 10, "0123456789");
		serialPutStr(", ErrCode: ");
		serialPutNbr(Rs.errCode, 10, "0123456789");
		serialPutStr("\r\n");
		ttyAddStr("\r\n[Fatal Error] Exception Raised => Nbr: ");
		ttyAddNbr(Rs.intNbr, 10, "0123456789");
		ttyAddStr(", ErrCode: ");
		ttyAddNbr(Rs.errCode, 10, "0123456789");
		__asm__ __volatile__ ("cli; hlt");
	}
}

void	initIdtEntries(){
	for (uint8_t i = 0; i < USED_HANDLERS; i++)
		initIdtEntry(i, handlers[i], 0x8E);
}

void	initIdt(){
	idtP.base = (uint32_t)&idtEntries;
	idtP.limit = sizeof(idtEntry) * 255; // * 256 - 1
	bzero(idtEntries, sizeof(idtEntry) * 255);
	picRemap(32, 40);
	initIdtEntries();

	__asm__ __volatile__ ("lidt (%%eax)": : "a"(&idtP));	// loading the idt into the IDTR.
	__asm__ __volatile__ ("sti");
}