#pragma once

#include "types.h"

# define VIDEO_MEMORY   0xB8000
# define VGA_WIDTH      80
# define VGA_HEIGHT     24 // status bar 
# define VGA_CELLS_COUNT (VGA_WIDTH * VGA_HEIGHT)

typedef union cellColor
{
	char bgColor:4;
    char fgColor:4;
    char clr;
} _cellColor;

typedef struct vgaCell{
    char c;
    _cellColor color;
} _vgaCell;

typedef struct cursor
{
    int8_t x;
    int8_t y;
}_cursor;

typedef struct ttySession
{
    _vgaCell screenBuff[VGA_WIDTH * VGA_HEIGHT];
    uint8_t defClr;
    _cursor cursor;
} _ttySession;

enum vgaColors{
    VGA_BLACK,
    VGA_BLUE,
    VGA_GREEN,
    VGA_CYAN,
    VGA_RED,
    VGA_MAGENTA,
    VGA_BROWN,
    VGA_LIGHT_GREY,
    VGA_DARK_GREY,
    VGA_LIGHT_BLUE,
    VGA_LIGHT_GREEN,
    VGA_LIGHT_CYAN,
    VGA_LIGHT_RED,
    VGA_LIGHT_MAGENTA,
    VGA_YELLOW,
    VGA_WHITE
};

uint8_t kPutPosC(char c, int8_t x, int8_t y);
uint8_t kPutPosS(char *s, int8_t x, int8_t y);
uint8_t kPutC(char c);
uint8_t kPutS(char *s);

uint8_t moveCursor(int8_t x, int8_t y);
uint8_t scroll();

uint8_t screenBuffOut();