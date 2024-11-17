#include "yonaData.h"
#include "klibc/print.h"
#include "drivers/time.h"
#include "klibc/strings.h"
#include "arch/i386/idts.h"
#include "drivers/keyboard.h"
#include "yonaShell/yonaShell.h"
#include "drivers/vga/textMode/vgaTextMode.h"

_ttySession tty;
_yonaData	yona = {
	.OSVersion = "0.7.1",
	.status = YONA_STATUS_STABLE,
	.isPaginated = false,
	.mainEBP = 0,
};
extern _sysClock date;

char *yonaStateToString(enum yonaStatus state){
	switch (state) {
		case YONA_STATUS_STABLE: return "\033[32mSTABLE\033[39m";
		case YONA_STATUS_ERROR: return "\033[93mERROR\033[39m";
		default: return "\033[31mUNKNOWN\033[39m";
	}
}

void updateStatusBar(){
	char content[80] = {0};

	clearStatusBar();
	SPRINTF(content, "| OSVersion: "COLOR_LIGHT_CYAN"%s"COLOR_DEFAULT" | STATE: %s |", yona.OSVersion, yonaStateToString(yona.status));
	kPutPosS(content, 0, VGA_HEIGHT);
	SPRINTF(content, "| %02d/%02d/%04d %02d:%02d:%02d |", date.d, date.mo, date.y, 
													date.h, date.m, date.s);
	kPutPosS(content, 57, VGA_HEIGHT);
}

void setYonaStatus(enum yonaStatus status){
	yona.status = status;
	updateStatusBar();
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

	setDate(16, 11, 2024);
	setTime(7, 7, 7);
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
	initShell();
}

void kmain(void) {
	__asm__ __volatile__("mov %%ebp, %0" : "=r" (yona.mainEBP) : : "memory");
	S_DEBUG("ebp %p\n", yona.mainEBP);
	kInits();
	volatile char s[] = "Hello this is the Main Function";
	volatile char *str = (char *)s; (void)str;

}