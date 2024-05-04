#include "vga.h"
#include "types.h"

void vga_puts(const char *s, int color){
    static size_t offset;
    char *vMemory = (char*)VIDEO_MEMORY + offset;
    while(*s)
    {
        vMemory[0] = *s;
        vMemory[1] = color;
        offset += 2;
        vMemory += 2;
        s++;
    }
}