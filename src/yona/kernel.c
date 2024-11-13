#include "klibc/print.h"
#include "drivers/time.h"
#include "drivers/keyboard.h"
#include "arch/i386/idts.h"
#include "drivers/vga/textMode/vgaTextMode.h"

_ttySession tty;
extern _sysClock date;

void updateStatusBar(){
	char content[80] = {0};

	SPRINTF(content, "| OSVersion: \033[96m%s\033[39m | STATE: \033[31m%s\033[39m |", "0.7.1", "UNSTABLE");
	kPutPosS(content, 0, VGA_HEIGHT);
	SPRINTF(content, "| %02d/%02d/%04d %02d:%02d:%02d |", date.d, date.mo, date.y, 
													date.h, date.m, date.s);
	kPutPosS(content, 54, VGA_HEIGHT);
}

void initTty(){
	tty.cursor.x = 0;
	tty.cursor.y = 0;

	for (size_t i = 0, cellsCount = VGA_WIDTH * VGA_HEIGHT; i < cellsCount; i++) {
		tty.screenBuff[i].c = 0;
		tty.screenBuff[i].color.clr = 0;
	}
	tty.currentColor = VGA_WHITE;
	tty.defClr = VGA_WHITE;
	enableCursor(14, 15);

	setDate(12, 11, 2024);
	updateStatusBar();
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
	initKeyboard();
	// initShell();
}

void kmain(void) {
	kInits();
	char buffer[256] = {0};

	while (1){
		prompt("Yona", buffer);
		PRINT_K("\n\r%s\n\r", buffer);
		S_DEBUG("Buffer: %s\r\n", buffer);
	}
}