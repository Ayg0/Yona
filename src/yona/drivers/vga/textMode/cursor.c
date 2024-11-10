#include "drivers/vga/textMode/vgaTextMode.h"
#include "klibc/memory.h"
#include "arch/serial.h"
#include "klibc/print.h"

extern _ttySession tty;

void enableCursor(uint8_t cursor_start, uint8_t cursor_end) {
	pByteOut(VGA_CTRL, 0x0A);
	pByteOut(VGA_DATA, (pByteIn(VGA_DATA) & 0xC0) | cursor_start);

	pByteOut(VGA_CTRL, 0x0B);
	pByteOut(VGA_DATA, (pByteIn(VGA_DATA) & 0xE0) | cursor_end);
}

void disableCursor() {
	pByteOut(VGA_CTRL, 0x0A);
	pByteOut(VGA_DATA, 0x20);
}

void updateCursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
    _vgaCell *cell = ((_vgaCell *)VIDEO_MEMORY) + pos;

	pByteOut(VGA_CTRL, 0x0F);
	pByteOut(VGA_DATA, (uint8_t) L8(pos));
	pByteOut(VGA_CTRL, 0x0E);
	pByteOut(VGA_DATA, (uint8_t) H8(pos));

    // making sure cursor is visible:
    cell->color.bgColor = tty.currentColor;
}

uint8_t moveCursor(int8_t x, int8_t y){
    if (x == VGA_WIDTH)
        x = 0, y++;
    if (y == VGA_HEIGHT)
        scroll(), y--;
    tty.cursor.x = x;
    tty.cursor.y = y;
    updateCursor(x, y);
    return 0;
}

uint8_t scroll(){

    for (size_t i = 0; i < VGA_HEIGHT - 1; i++)
        memmove(&tty.screenBuff[i * VGA_WIDTH], &tty.screenBuff[(i + 1) * VGA_WIDTH], VGA_WIDTH * 2);
    memset(&tty.screenBuff[(VGA_HEIGHT - 1) * VGA_WIDTH], 0, VGA_WIDTH * 2);
    screenBuffOut();
    return 0;
}
