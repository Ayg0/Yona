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
	tty.defClr = VGA_WHITE;
}

void kmain(void) 
{
	initTty();
	tty.defClr = VGA_GREEN;
	char c = 'A';
	tty.cursor.y = 20;
	while (1){
		kPutC(c);
		c++;
		if (c > 'Z') c = 'A';
		for (size_t i = 0; i < 7000; i++){
			for (size_t j = 0; j < 7000; j++){}
		}
		
	}
}