#include "vga.h"

void kmain(){
	// vga_puts("Hello", VGA_LIGHT_BLUE);
	// vga_puts(", World", VGA_LIGHT_GREEN);
	// vga_puts("!!!", VGA_LIGHT_BROWN);
	vga_puts("42", VGA_LIGHT_RED);
	while(1);
}