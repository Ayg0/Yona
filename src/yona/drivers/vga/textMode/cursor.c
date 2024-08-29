#include "drivers/vga/textMode/vgaTextMode.h"
#include "klibc/memory.h"

extern _ttySession tty;

uint8_t moveCursor(int8_t x, int8_t y){
    if (x == VGA_WIDTH)
        x = 0, y++;
    if (y == VGA_HEIGHT)
        scroll(), y--;
    tty.cursor.x = x;
    tty.cursor.y = y;
    return 0;
}

uint8_t scroll(){

    for (size_t i = 0; i < VGA_HEIGHT - 1; i++)
        memmove(&tty.screenBuff[i * VGA_WIDTH], &tty.screenBuff[(i + 1) * VGA_WIDTH], VGA_WIDTH * 2);
    memset(&tty.screenBuff[(VGA_HEIGHT - 1) * VGA_WIDTH], 0, VGA_WIDTH * 2);
    screenBuffOut();
    return 0;
}
