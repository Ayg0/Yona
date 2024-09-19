#include "drivers/vga/textMode/vgaTextMode.h"
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

void kmain(void) {
	initTty();
	// initGdt();
	// initIdt();
	// initKeyboard();
	// initShell();
	// newShell();
}