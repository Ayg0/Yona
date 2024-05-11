#include "CPU/DiscriptorTables.h"
#include "ports.h"
#include "serialPorts.h"
/* R
	https://wiki.osdev.org/PIC
	https://pdos.csail.mit.edu/6.828/2010/readings/hardware/8259A.pdf
*/
/*
	we need to remap the IRQs of the pic slave and master
	before remapping:
		IRQs 0-7  ->  IDT_ENTRYs    8-15
		IRQs 8-15 ->  IDT_ENTRYs 0x70-0x78
	we want to map it to:
		IRQs 0-15 ->  IDT_ENTRYs    32-47
*/
# define MASTER_CTRL	0x20
# define MASTER_DATA	0x21
# define SLAVE_CTRL		0xA0
# define SLAVE_DATA		0xA1
#define	 PIC_EOI		0x20

void *IRQRoutines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
// set a handler to the IRQ
void	setIRQHandler(uint8_t irq, void (*handler)(registers r)){
	IRQRoutines[irq] = handler;
}
// reset the handler to NULL
void	clearIRQHandler(uint8_t irq){
	IRQRoutines[irq] = 0;
}
// send End of Interrupt to the Pics
void	picEOI(uint8_t irq){
	if (irq >= 8)
		PbyteOut(SLAVE_CTRL, PIC_EOI);
	PbyteOut(MASTER_CTRL, PIC_EOI);
}

void	irqHandler(registers Rs){
	void (*f)(registers);
	uint8_t irq = Rs.intNbr - 32;

	if (irq < 16 && IRQRoutines[irq]){
		f = (void *)IRQRoutines[irq];
		f(Rs);
	}
	picEOI(irq);
}

void picRemap(uint8_t masterOffset, uint8_t slaveOffset){
	uint8_t a1, a2;
 
	a1 = PbyteIn(MASTER_DATA);				// save masks
	a2 = PbyteIn(SLAVE_DATA);
 
	PbyteOut(MASTER_CTRL, 0x10 | 0x01);		// starts the initialization sequence (in cascade mode)
	ioWait();
	PbyteOut(SLAVE_CTRL, 0x10 | 0x01);
	ioWait();
	PbyteOut(MASTER_DATA, masterOffset);	// ICW2: Master PIC vector offset
	ioWait();
	PbyteOut(SLAVE_DATA, slaveOffset);		// ICW2: Slave PIC vector offset
	ioWait();
	PbyteOut(MASTER_DATA, 4);				// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	ioWait();
	PbyteOut(SLAVE_DATA, 2);				// ICW3: tell Slave PIC its cascade identity (0000 0010)
	ioWait();
 
	PbyteOut(MASTER_DATA, 0x01);			// ICW4: have the PICs use 8086 mode (and not 8080 mode)
	ioWait();
	PbyteOut(SLAVE_DATA, 0x01);
	ioWait();

	PbyteOut(MASTER_DATA, a1);				// restore saved masks.
	PbyteOut(SLAVE_DATA, a2);
}