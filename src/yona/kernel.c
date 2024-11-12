#include "klibc/print.h"
#include "drivers/time.h"
#include "arch/i386/idts.h"
#include "drivers/vga/textMode/vgaTextMode.h"

_ttySession tty;

extern _sysClock date;

void makeStatusBar(){
	char statusBar[80] = {0};

	// status bar will have OS version, time, date, and OS state;

	SPRINTF(statusBar, "| OSVersion:%s | STATE:%s |", "0.1.2", "UNSTABLE");
	SPRINTF((statusBar + 53), "| %02d/%02d/%04d %02d:%02d:%02d |", date.d, date.mo, date.y, 
													date.h, date.m, date.s);
	PRINT_K("| %02d/%02d/%04d %02d:%02d:%02d |", date.d, date.mo, date.y, 
													date.h, date.m, date.s);
	PRINT_K("| %2d/%2d/%4d %2d:%2d:%2d |\r\n", date.d, date.mo, date.y, 
													date.h, date.m, date.s);												
	for (uint8_t i = 0; i < 79; i++)
	{
		if (statusBar[i] == 0)
			statusBar[i] = ' ';
	}
	statusBar[79] = 0;
	updateStatusBar(statusBar);
}

void initTty(){
	tty.cursor.x = 0;
	tty.cursor.y = 0;

	for (size_t i = 0, cellsCount = VGA_WIDTH * VGA_HEIGHT; i < cellsCount; i++)
	{
		tty.screenBuff[i].c = 0;
		tty.screenBuff[i].color.clr = 0;
	}
	tty.currentColor = VGA_WHITE;
	tty.defClr = VGA_WHITE;
	enableCursor(14, 15);
	makeStatusBar();
}

void gdtTest() {
    char*		addr;
	uint32_t	cr0;
	_gdtPtr		gdt = {.base = 0, .limit = 0};

	S_INFO("TESTING DGT\r\n", NULL);
	/*gdt*/
	asm volatile("sgdt %0" : : "m" (gdt));
	S_DEBUG("GDT Base Address: %8p\r\n", gdt.base);
	S_DEBUG("GDT Limit: %8p\r\n", gdt.limit);

    /* Test code segment */
    asm volatile("mov %%cs, %0" : "=r" (addr));
    S_DEBUG("Code Segment Base Address: %8p\r\n", addr);

    /* Test data segment */
    asm volatile("mov %%ds, %0" : "=r" (addr));
	S_DEBUG("data Segment Base Address: %8p\r\n", addr);

    /* Test stack segment */
    asm volatile("mov %%ss, %0" : "=r" (addr));
    S_DEBUG("Stack Segment Base Address: %8p\r\n", addr);
	/*Testing if paging Enabled*/
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	S_DEBUG("Paging is currently: %s\r\n", (cr0 & 0x80000000) ? "ENABLED": "DISABLED");
	S_INFO("END TESTING GDT\r\n", NULL);
}
void	initIdt();

void	kInits(){
	gdtTest();
	initGdt();
	gdtTest();
	initIdt();
	initTty();
	initTimer(1000);
	// initKeyboard();
	// initShell();
}

void	keyboardHandler(_registers Rs){
	(void) Rs;
	uint8_t	letter = 0;
    uint8_t scanCode =  pByteIn(0x60); // where PIC leave the scancode

	if (scanCode < 0x80){
		letter = scanCode;
		kPutC(letter);
	}
}

void kmain(void) {
	kInits();
	setIRQHandler(1, keyboardHandler);
	while (1);
}