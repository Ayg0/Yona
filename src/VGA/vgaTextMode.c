#include "vga.h"
#include "types.h"

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
    vMemory[ *x + *y * 80] = color << 8 | c;
}