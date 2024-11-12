#include "klibc/print.h"
#include "arch/ports.h"
#include "arch/i386/idts.h"
/*
	we need to remap the IRQs of the pic slave and master
	before remapping:
		IRQs 0-7  ->  IDT_ENTRYs    8-15
		IRQs 8-15 ->  IDT_ENTRYs    0x70-0x78
	we want to map it to:
		IRQs 0-15 ->  IDT_ENTRYs    32-47
*/

void *IRQRoutines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
// set a handler to the IRQ
void	setIRQHandler(uint8_t irq, void (*handler)(_registers r)){
	IRQRoutines[irq] = handler;
	S_INFO("IRQ Handler set for IRQ %d\n", irq);
}
// reset the handler to NULL
void	clearIRQHandler(uint8_t irq){
	IRQRoutines[irq] = 0;
	S_INFO("IRQ %d handler cleared\n", irq);
}
// send End of Interrupt to the Pics
void	picEOI(uint8_t irq){
	if (irq >= 8)
		pByteOut(SLAVE_CTRL, PIC_EOI);
	pByteOut(MASTER_CTRL, PIC_EOI);
}

// IRQ handler
void	irqHandler(_registers Rs){
	void (*f)(_registers);
	uint8_t irq = Rs.intNbr - 32;

	if (irq < 16 && IRQRoutines[irq])
		f = (void *)IRQRoutines[irq], f(Rs);
	else
		S_ERR("IRQ %d not handled\n", irq);
	picEOI(irq);
}

void picRemap(uint8_t masterOffset, uint8_t slaveOffset){
	uint8_t a1, a2;
 
	a1 = pByteIn(MASTER_DATA);				// save masks
	a2 = pByteIn(SLAVE_DATA);
 
	pByteOut(MASTER_CTRL, 0x10 | 0x01);		// starts the initialization sequence (in cascade mode)
	ioWait();
	pByteOut(SLAVE_CTRL, 0x10 | 0x01);
	ioWait();
	pByteOut(MASTER_DATA, masterOffset);	// ICW2: Master PIC vector offset
	ioWait();
	pByteOut(SLAVE_DATA, slaveOffset);		// ICW2: Slave PIC vector offset
	ioWait();
	pByteOut(MASTER_DATA, 4);				// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	ioWait();
	pByteOut(SLAVE_DATA, 2);				// ICW3: tell Slave PIC its cascade identity (0000 0010)
	ioWait();
 
	pByteOut(MASTER_DATA, 0x01);			// ICW4: have the PICs use 8086 mode (and not 8080 mode)
	ioWait();
	pByteOut(SLAVE_DATA, 0x01);
	ioWait();

	pByteOut(MASTER_DATA, a1);				// restore saved masks.
	pByteOut(SLAVE_DATA, a2);
}