#pragma once
# include "types.h"

/*  R
	https://wiki.osdev.org/VGA_Hardware
	https://wiki.osdev.org/Text_Mode_Cursor
	https://wiki.osdev.org/Text_UI
	https://wiki.osdev.org/VGA_Resources
*/

# define VIDEO_MEMORY 0xB8000
# define VGA_WIDTH 80
# define VGA_HEIGHT 25

// DEVICE I/O ports
# define VGA_CTRL 0x3D4
/*
	this is one of the most used indexed registers.
	The index byte is written to the port given, then the data byte
	can be read/written from/to port+1 (0x3D5)
*/
# define VGA_DATA 0x3D5
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
# define GET_CHAR(CHAR, COLOR) ((COLOR << 8) | (CHAR & 0xff))

void kputs(const char *s, int x, int y, uint8_t color);
void kputC(const char c, int *x, int *y, uint8_t color);

// cursor:
void		enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void		disable_cursor();
void		update_cursor(int x, int y);
uint16_t	get_cursor_position(void);