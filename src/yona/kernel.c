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
}

void kmain(void) 
{
	initTty();
	
	char s[20] = {0};
	SPRINTF(s, "Hello %d;%s;%p\r\n", 77, "Taha", 0x445);
	kPutPosS(s, 0, VGA_HEIGHT);
}