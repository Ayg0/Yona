#include "drivers/vga/textMode/vgaTextMode.h"
#include "klibc/memory.h"
extern _ttySession tty;

uint8_t kPutPosC(char c, int8_t x, int8_t y){
    int16_t pos = x + (y * VGA_WIDTH);
    tty.screenBuff[pos].c = c;
    tty.screenBuff[pos].color.clr = tty.defClr;

    ((short*)VIDEO_MEMORY)[pos] = tty.screenBuff[pos].color.clr << 8 | c;
    return 0;
}

uint8_t kPutC(char c){
    kPutPosC(c, tty.cursor.x, tty.cursor.y);
    moveCursor(tty.cursor.x + 1, tty.cursor.y);
    return 0;
}

uint8_t kPutPosS(char *s, int8_t x, int8_t y){
    while (*s)
    {
        kPutPosC(*s, x, y);
        x++;
        if (x == VGA_WIDTH)
            x = 0, y++;
        if (y == VGA_HEIGHT){
            scroll(), y--;
        }
        s++;
    }
    return 0;
}

uint8_t kPutS(char *s){
    while (*s)
        kPutC(*s), s++;
    return 0;
}

uint8_t screenBuffOut(){
    _vgaCell *vgaMem = (_vgaCell *)VIDEO_MEMORY;
    int     copyLen = VGA_WIDTH * 2;

    for (int16_t i = 0; i < VGA_CELLS_COUNT; i++)
        vgaMem[i] = tty.screenBuff[i];

    for (size_t i = 0; i < VGA_HEIGHT; i++){
        int copyStartIndex = i * VGA_WIDTH;
        memmove(&vgaMem[copyStartIndex], &tty.screenBuff[copyStartIndex], copyLen);
    }
    return 0;
}