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
	uint8_t i = 1;
	while (1)
	{
		changeTtyColor(i, -1);
		i++;
		if (i == 16)
			i = 1;
		ttyAddStr("Hello World\r\n");
		fake_sleep(10000);
	}
}