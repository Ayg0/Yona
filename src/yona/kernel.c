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

	// char c = 'A';
	// moveCursor(VGA_WIDTH - 5, VGA_HEIGHT - 1);
	// K_PRINT("%s", "\033[34m");
	// while (1)
	// {
	// 	for (size_t i = 0; i < 9000; i++)
	// 		for (size_t j = 0; j < 9000; j++) {}
	// 	kPutC(c);
	// 	c = c == 'Z' ? 'A' : (c + 1);
	// }
	
	char s[20] = {0};
	SPRINTF(s, "Hello %d;%s;%p\r\n", 77, "Taha", 0x445);
	PRINT_K("OK: %s", s);
}