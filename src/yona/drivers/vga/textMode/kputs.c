#include "drivers/vga/textMode/vgaTextMode.h"
#include "klibc/memory.h"
#include "klibc/converts.h"
#include "klibc/strings.h"
extern _ttySession tty;

uint8_t ansitoVgaColors[] = {
	30, // VGA_BLACK
	34, // VGA_BLUE
	32, // VGA_GREEN
	36, // VGA_CYAN
	31, // VGA_RED
	35, // VGA_MAGENTA
	33, // VGA_BROWN
	37, // VGA_LIGHT_GREY
	90, // VGA_DARK_GREY
	94, // VGA_LIGHT_BLUE
	92, // VGA_LIGHT_GREEN
	96, // VGA_LIGHT_CYAN
	91, // VGA_LIGHT_RED
	95, // VGA_LIGHT_MAGENTA
	93, // VGA_YELLOW
	97  // VGA_WHITE	
};

uint8_t getVgaColor(uint8_t color){
    int16_t i = 0;
    while (i < 16){
        if (ansitoVgaColors[i] == color)
            break;
        i++;
    }
    return i;
}

void parseAnsiSequence(char *ansiBuff, uint8_t *ansiBuffSize){
    switch (ansiBuff[*ansiBuffSize - 1]){
        case 'm':
            tty.currentColor = getVgaColor(atoiS(strrchr(ansiBuff, '['), NULL));
            break;
        case 'H':
            break;
        default:
            break;
    }
    if (tty.currentColor == 16) // just in case it's a bad color
        tty.currentColor = tty.defClr;
    memset(ansiBuff, 0, *ansiBuffSize);
    *ansiBuffSize = 0;
}

uint8_t handleSpecialChars(char c, char *ansiBuff, uint8_t *ansiBuffSize){
    
    switch (c)
    {
    case '\033':
        ansiBuff[*ansiBuffSize] = c, (*ansiBuffSize)++;
        return 1;
    case '\r':
        moveCursor(0, tty.cursor.y);
        return 1;
    case '\n':
        moveCursor(tty.cursor.x, tty.cursor.y + 1);
        return 1;
    default:
        break;
    }
    return 0;
}

uint8_t kPutPosC(char c, int8_t x, int8_t y){
    int16_t pos = x + (y * VGA_WIDTH);
    static char     ansiBuff[20];
    static uint8_t  ansiBuffSize;

    if (ansiBuffSize){
        ansiBuff[ansiBuffSize] = c, ansiBuffSize++;
        if (c == 'm' || c == 'H')
            parseAnsiSequence(ansiBuff, &ansiBuffSize);
        return 0;
    }
    if (handleSpecialChars(c, ansiBuff, &ansiBuffSize))
        return 0;

    tty.screenBuff[pos].c = c;
    tty.screenBuff[pos].color.clr = tty.currentColor;

    ((short*)VIDEO_MEMORY)[pos] = tty.screenBuff[pos].color.clr << 8 | c;
    return 1;
}

uint8_t kPutC(char c){
    if (!kPutPosC(c, tty.cursor.x, tty.cursor.y))
    	return 0;
    moveCursor(tty.cursor.x + 1, tty.cursor.y);
	return 1;
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