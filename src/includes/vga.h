#pragma once
# include "types.h"

# define VIDEO_MEMORY 0xB8000

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

// get color from foreground and background colors
# define GET_COLOR(FG_COLOR, BG_COLOR) ((BG_COLOR << 4) | (FG_COLOR & 0x0f))
// insert char With it's colors
# define GET_CHAR(CHAR, COLOR) ((COLOR << 8) | CHAR)

void kputs(const char *s, int x, int y, uint8_t color);
void kputC(const char c, int *x, int *y, uint8_t color);
