#include "vga.h"

void kmain(){
	kputs("Hello", 0, 0, GET_COLOR(VGA_YELLOW, VGA_DARK_GREY));
	kputs("42", 79, 0, VGA_LIGHT_GREEN);
	while(1);
}