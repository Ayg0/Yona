#include "vga.h"
#include "tty.h"
#include "str.h"
tty	mainTty;

void fake_sleep(uint32_t iters){
	for (size_t volatile i = 0; i < iters; i++)
		for (size_t j = 0; j < iters; j++)
			i = i;
}

void kmain(){
	initTty(&mainTty);
	setTtyColor(VGA_LIGHT_GREEN, -1);
	sToSession("Hello World", 0, 0);
	selectSession(1);
	setTtyColor(VGA_YELLOW, -1);
	sToSession("potatoesssss", 0, 0);
	while(1){
		selectSession(0);
		printSessionContent();
		fake_sleep(20000);
		selectSession(1);
		printSessionContent();
		fake_sleep(20000);
	}
}