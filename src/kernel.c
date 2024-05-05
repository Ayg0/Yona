#include "vga.h"
#include "tty.h"
#include "str.h"

_tty	mainTty;

void fake_sleep(uint32_t iters){
	for (size_t volatile i = 0; i < iters; i++)
		for (size_t j = 0; j < iters; j++)
			i = i;
}

void kmain(){
	initTty();
	ttyAddStr("Hello World");
	// switchSession(1);
	// changeTtyColor(VGA_LIGHT_GREEN, -1);
	// ttyAddStr("Potatoes");
	// while (1)
	// {
	// 	switchSession(0);
	// 	fake_sleep(20000);
	// 	switchSession(1);
	// 	fake_sleep(20000);
	// }
}