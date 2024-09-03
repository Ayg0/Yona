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

	// test appendingWidth %d %x %u %p
	K_PRINT("test %5d\r\n", -53);
	K_PRINT("test %5x\r\n", 0x53);
	K_PRINT("test %5u\r\n", 53);
	K_PRINT("test %5p\r\n", 0x53);

	S_DEBUG("This is a debug message 0x%x\r\n", 0xFFA7);
	S_INFO("This is an info message 0x%x\r\n", 0xFFFF);
	S_SUCC("This is a success message 0x%x\r\n", 0xAB30);
	S_ERR("This is an error message 0x%x\r\n", 0x0);
}