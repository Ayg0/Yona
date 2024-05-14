#include "tty.h"
#include "str.h"
#include "timer.h"
#include "kLibStd.h"
#include "serialPorts.h"
#include "CPU/DiscriptorTables.h"
#include "keyboard.h"

_tty	mainTty;
uint32_t ticks;

extern gdtEntry	gdtEntries[GDT_ENTRIES];

void fake_sleep(uint32_t iters) __attribute__((deprecated));

void fake_sleep(uint32_t iters){
	for (size_t volatile i = 0; i < iters; i++)
		for (size_t j = 0; j < iters; j++)
			i = i;
}

void gdtTest() {
    char*		addr;
	uint32_t	cr0;

	serialPutStr("[DEBUG]: TESTING GDT\r\n");
    /* Test data segment */
    asm volatile("mov %%ds, %0" : "=r" (addr));
    serialPutStr("Data Segment Base Address: ");
	serialPutNbr((uint32_t) addr, 10, "0123456789");
	serialPutStr("\r\n");

    /* Test code segment */
    asm volatile("mov %%cs, %0" : "=r" (addr));
    serialPutStr("Code Segment Base Address: ");
	serialPutNbr((uint32_t) addr, 10, "0123456789");
	serialPutStr("\r\n");
    /* Test stack segment */
    asm volatile("mov %%ss, %0" : "=r" (addr));
    serialPutStr("Stack Segment Base Address: ");
	serialPutNbr((uint32_t) addr, 10, "0123456789");
	serialPutStr("\r\n");
	/*Testing if paging Enabled*/
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	serialPutStr("[INFO]: Paging is currently: ");
	if (cr0 & 0x80000000)
	    serialPutStr("ENABLED\r\n");
	else
	    serialPutStr("DISABLED\r\n");
	serialPutStr("[DEBUG]: END TESTING GDT\r\n");
}

void kernelInits(){
	initSerial();
	serialPutStr("[INFO]: SERIAL INIT SUCCESS\r\n");
	initDTs();
	gdtTest();
	serialPutStr("[INFO]: DISCRIPTOR TABLES INIT SUCCESS\r\n");
	initTty();
	serialPutStr("[INFO]: TTY INIT SUCCESS\r\n");
	init_timer(50);	
	serialPutStr("[INFO]: TIMER INIT SUCCESS\r\n");
	setIRQHandler(1, keyboardHandler);
}
void kmain(){
	kernelInits();
	// printfTty("Hello my name is %s, nice to see all %u of You !%s\r\n", "Taha", 700, "!!");
	char s[100];
	char *str = &s[0];
	setDate(13, 5, 2024);
	// formatString("hello %s, there are %d issues HAHA", &str, "Taha", 0);
	formatString("hello %s, there are %5d issues HAHA", &str, "Taha", 6);
	printfTty("s: %s\r\n", s);
}