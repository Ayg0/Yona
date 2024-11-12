#include "drivers/vga/textMode/vgaTextMode.h"
#include "arch/i386/idts.h"
#include "klibc/print.h"

_ttySession tty;

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
	updateStatusBar("Stat: \033[91mUnknown\033[0m | Battery: \033[91mUnknown\033[0m | Stability: \033[93mReally Nigga ?\033[0m | OSVersion: \033[36m0.1.2\033[0m");
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
	// initKeyboard();
	// initShell();
}

void	keyboardHandler(_registers Rs){
	(void) Rs;
	uint8_t	letter = 0;
    uint8_t scanCode =  pByteIn(0x60); // where PIC leave the scancode

	if (scanCode < 0x80)
	{
		letter = scanCode;
		kPutC(letter);
	}
}

void	doNothing(_registers Rs){
	(void) Rs;
}

void kmain(void) {
	kInits();
	setIRQHandler(0, doNothing);
	setIRQHandler(1, keyboardHandler);
	while (1);
}