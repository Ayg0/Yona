#include "tty.h"
#include "str.h"
#include "timer.h"
#include "kLibStd.h"
#include "serialio.h"
#include "CPU/DiscriptorTables.h"
#include "keyboard.h"
void	shell();

_tty	tty;
uint32_t ticks;

void gdtTest() {
    char*		addr;
	uint32_t	cr0;
	gdtPtr		gdt = {.base = 0, .limit = 0};

	SERIAL_INFO("TESTING DGT\r\n", NULL);
	/*gdt*/
	asm volatile("sgdt %0" : : "m" (gdt));
	SERIAL_DEBUG("GDT Base Address: %x\r\n", gdt.base);
	SERIAL_DEBUG("GDT Limit: %x\r\n", gdt.limit);

    /* Test data segment */
    asm volatile("mov %%ds, %0" : "=r" (addr));
	SERIAL_DEBUG("data Segment Base Address: %x\r\n", addr);

    /* Test code segment */
    asm volatile("mov %%cs, %0" : "=r" (addr));
    SERIAL_DEBUG("Code Segment Base Address: %x\r\n", addr);
    /* Test stack segment */
    asm volatile("mov %%ss, %0" : "=r" (addr));
    SERIAL_DEBUG("Stack Segment Base Address: %x\r\n", addr);
	/*Testing if paging Enabled*/
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	SERIAL_INFO("Paging is currently: %s\r\n", cr0 & 0x80000000 ? "ENABLED": "DISABLED");
	SERIAL_INFO("END TESTING GDT\r\n", NULL);
	initKeboard(0);
}

void kernelInits(){
	initSerial();
	SERIAL_SUCC("%s INIT SUCCESS\r\n", "SERIAL");
	initDTs();
	gdtTest();
	SERIAL_SUCC("%s INIT SUCCESS\r\n", "DISCRIPTOR TABLES");
	initTty();
	SERIAL_SUCC("%s INIT SUCCESS\r\n", "TTY");
	init_timer(1000);	
	SERIAL_SUCC("%s INIT SUCCESS\r\n", "TIMER");
	setIRQHandler(1, keyboardHandler);
}
extern uint32_t mainEBP;
void kmain(){
	kernelInits();
    __asm__ __volatile__("mov %%ebp, %0" : "=r" (mainEBP) : : "memory");
	volatile char s[] = "Hello this is the Main Function";
	char *str = (char *)s; (void)str;
	
	uint8_t clr = tty.color;
	changeTtyColor(VGA_YELLOW, -1);
	SERIAL_SUCC("Session %d INIT\r\n", 1);
	for (uint8_t i = 1; i < 5; i++)	// just to init the other Sessions as shells
	{
		switchSession(i);
		SERIAL_SUCC("Session %d INIT\r\n", i + 1);
		ttyAddStr("$> ");
	}
	tty.color = clr;
	switchSession(0);
	setDate(1, 1, 2000);
	shell();
}