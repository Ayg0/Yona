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
}

void kmain(void) 
{
	initTty();
	K_PRINT("\r\n%d\r\n", K_PRINT("\033[1mHello World !!\033[16m\r\n%s;%u;%s;%d;%x", "Hiiii", 97, NULL, -5, 0xf));
	S_PRINT("\033[1mHello World !!\033[16m\r\n%s;%u;%s;%d;%x", "Hiiii", 97, NULL, -5, 0xf);
	// K_PRINT("\r\n%d\r\n", K_PRINT("Hello !", NULL));
}