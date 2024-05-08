#include "vga.h"
#include "tty.h"
#include "str.h"
#include "serialPorts.h"
#include "CPU/DiscriptorTables.h"
_tty	mainTty;

void fake_sleep(uint32_t iters) __attribute__((deprecated));

void fake_sleep(uint32_t iters){
	for (size_t volatile i = 0; i < iters; i++)
		for (size_t j = 0; j < iters; j++)
			i = i;
}

void kmain(){
	initSerial();
	serialPutStr("[INFO]: SERIAL INIT SUCCESS\r\n");
	initTty();
	serialPutStr("[INFO]: TTY INIT SUCCESS\r\n");
	// initDTs();
	// printTtySession();
	// fake_sleep(10000);
	setTtyCursor(0, 20);
	ttyAddStr("Hello");	
	while (1)
	{
		setTtyCursor(mainTty.currentSession->cursor.x + 1, mainTty.currentSession->cursor.y);
		fake_sleep(5000);
	}
	
}