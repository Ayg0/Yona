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
	K_PRINT("\033[34mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[31mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[32mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[33mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[35mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[36mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[37mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[90mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[91mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[92mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[93mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[94mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[95mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[96mHello World !!\033[39m\r\n", NULL);
	K_PRINT("\033[97mHello World !!\033[39m\r\n", NULL);

	S_PRINT("\033[34mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[31mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[32mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[33mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[35mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[36mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[37mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[90mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[91mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[92mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[93mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[94mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[95mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[96mHello World !!\033[39m\r\n", NULL);
	S_PRINT("\033[97mHello World !!\033[39m\r\n", NULL);

	// K_PRINT("\r\n%d\r\n", K_PRINT("Hello !", NULL));
}