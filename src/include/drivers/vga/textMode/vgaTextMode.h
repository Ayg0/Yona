#pragma once

#include "klibc/types.h"

# define VIDEO_MEMORY   0xB8000
# define VGA_WIDTH      80
# define VGA_HEIGHT     24 // -1 for the status bar 
# define VGA_CELLS_COUNT (VGA_WIDTH * VGA_HEIGHT)

// DEVICE I/O ports
# define VGA_CTRL 0x3D4
/*
	this is one of the most used indexed registers.
	The index byte is written to the port given, then the data byte
	can be read/written from/to port+1 (0x3D5)
*/
# define VGA_DATA 0x3D5

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
    uint8_t currentColor;
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

// basic prints
uint8_t kPutPosC(char c, int8_t x, int8_t y);
uint8_t kPutPosS(char *s, int8_t x, int8_t y);
uint8_t kPutC(char c);
uint8_t kPutS(char *s);

// cursor
uint8_t scroll();
uint8_t moveCursor(int8_t x, int8_t y);
void    enableCursor(uint8_t cursor_start, uint8_t cursor_end);

// output the screen-buff
uint8_t screenBuffOut();
void    clearStatusBar();
void    clearScreenBuffer();