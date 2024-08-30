#include "drivers/vga/textMode/vgaTextMode.h"
#include "klibc/memory.h"
#include "klibc/converts.h"
#include "klibc/strings.h"
extern _ttySession tty;

void parseAnsiSequence(char *ansiBuff, uint8_t *ansiBuffSize){
    switch (ansiBuff[*ansiBuffSize - 1]){
        case 'm':
            tty.currentColor = atoiS(strrchr(ansiBuff, '['), NULL);
            break;
        case 'H':
            break;
        default:
            break;
    }
    if (tty.currentColor == 16)
        tty.currentColor = tty.defClr;
    memset(ansiBuff, 0, *ansiBuffSize);
    *ansiBuffSize = 0;
}

uint8_t kPutPosC(char c, int8_t x, int8_t y){
    int16_t pos = x + (y * VGA_WIDTH);
    static char     ansiBuff[20];
    static uint8_t  ansiBuffSize;

    if (c == '\033' || ansiBuffSize){
        ansiBuff[ansiBuffSize] = c, ansiBuffSize++;
        if (c == 'm' || c == 'H')
            parseAnsiSequence(ansiBuff, &ansiBuffSize);
        return 0;
    }
    tty.screenBuff[pos].c = c;
    tty.screenBuff[pos].color.clr = tty.currentColor;

    ((short*)VIDEO_MEMORY)[pos] = tty.screenBuff[pos].color.clr << 8 | c;
    return 1;
}

uint8_t kPutC(char c){
    if (kPutPosC(c, tty.cursor.x, tty.cursor.y))
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