#include "drivers/vga/textMode/vgaTextMode.h"

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

	kPutS("\033[1mHello World !!\033[16m");
	kPutS("\033[13mHello World !!\033[16m");
	kPutS("Hello World !!");
}