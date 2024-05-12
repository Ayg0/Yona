#include "vga.h"
#include "tty.h"
#include "str.h"
#include "serialPorts.h"
#include "CPU/DiscriptorTables.h"
#include "types.h"
#include "kLibStd.h"

_tty	mainTty;

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

void timerFunction(registers Rs){
	static uint32_t ticks;

	(void)Rs;
	ticks++;
	_pos pastPos =  mainTty.currentSession->cursor;
	if (!(ticks % 5)){
		setTtyCursor(0, 0);
		ttyAddStr("Ticks: ");
		ttyAddNbr(ticks, 10, "0123456789");
		mainTty.currentSession->cursor = pastPos;
	}
}

void kernelInits(){
	initSerial();
	serialPutStr("[INFO]: SERIAL INIT SUCCESS\r\n");
	initDTs();
	gdtTest();
	// setIRQHandler(0, timerFunction);
	serialPutStr("[INFO]: DISCRIPTOR TABLES INIT SUCCESS\r\n");
	initTty();
	serialPutStr("[INFO]: TTY INIT SUCCESS\r\n");
}

void kmain(){
	kernelInits();
	// printfTty("Hello my name is %s, nice to see all %u of You !%s\r\n", "Taha", 700, "!!");
	char s[100];
	char *str = &s[0];
	formatString("hello %s, there are %d issues HAHA", &str, "Taha", 0);
	printfTty("s: %s\r\n", s);
}