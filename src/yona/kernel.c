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

	K_PRINT("Hi %s\r\n", "Taha");

	S_DEBUG("THIS IS A DEBUG MSG\r\n", NULL);
	S_INFO("THIS IS AN INFO MSG\r\n", NULL);
	S_SUCC("THIS IS A SUCC MSG\r\n", NULL);
	S_ERR("THIS IS AN ERR MSG\r\n", NULL);
}