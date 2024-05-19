#include "vga.h"
#include "types.h"
#include "ports.h"

void kputs(const char *s, int x, int y, uint8_t color){
    while(*s)
    {
        kputC(*s, &x, &y, color);
        x++;
        s++;
    }
}

void kputC(const char c, int *x, int *y, uint8_t color){
    uint16_t *vMemory = (uint16_t*)VIDEO_MEMORY;
    if (*x >= 80)
        *x = 0, (*y)++;
    vMemory[ *x + *y * 80] = GET_CHAR(c, color);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	PbyteOut(VGA_CTRL, 0x0A);
	PbyteOut(VGA_DATA, (PbyteIn(VGA_DATA) & 0xC0) | cursor_start);
 
	PbyteOut(VGA_CTRL, 0x0B);
	PbyteOut(VGA_DATA, (PbyteIn(VGA_DATA) & 0xE0) | cursor_end);
}

void disable_cursor()
{
	PbyteOut(VGA_CTRL, 0x0A);
	PbyteOut(VGA_DATA, 0x20);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
	uint16_t *vidM = (uint16_t *)VIDEO_MEMORY;
	uint8_t	cellColor = H8(vidM[pos]);
 
	PbyteOut(VGA_CTRL, 0x0F);
	PbyteOut(VGA_DATA, (uint8_t) (pos & 0xFF));
	PbyteOut(VGA_CTRL, 0x0E);
	PbyteOut(VGA_DATA, (uint8_t) ((pos >> 8) & 0xFF));
	if (!cellColor)
		vidM[pos] = GET_CHAR(vidM[pos], GET_COLOR(VGA_WHITE, VGA_BLACK));	// make sure that the cursor is visible;
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    PbyteOut(VGA_CTRL, 0x0F);
    pos |= PbyteIn(VGA_DATA);
    PbyteOut(VGA_CTRL, 0x0E);
    pos |= ((uint16_t)PbyteIn(VGA_DATA)) << 8;
    return pos;
}