#include "tty.h"
#include "str.h"
#include "timer.h"
#include "kLibStd.h"
#include "serialio.h"
#include "CPU/DiscriptorTables.h"
#include "keyboard.h"
void	shell();

_tty	tty;
uint32_t ticks;

// void fake_sleep(uint32_t iters) __attribute__((deprecated));

void fake_sleep(uint32_t iters){
	for (size_t volatile i = 0; i < iters; i++)
		for (size_t j = 0; j < iters; j++)
			i = i;
}

void gdtTest() {
    char*		addr;
	uint32_t	cr0;

    /* Test data segment */
    asm volatile("mov %%ds, %0" : "=r" (addr));
	SERIAL_DEBUG("TESTING DGT\r\n data Segment Base Address: %x\r\n", addr);

    /* Test code segment */
    asm volatile("mov %%cs, %0" : "=r" (addr));
    SERIAL_DEBUG("Code Segment Base Address: %x\r\n", addr);
    /* Test stack segment */
    asm volatile("mov %%ss, %0" : "=r" (addr));
    SERIAL_DEBUG("Stack Segment Base Address: %x\r\n", addr);
	/*Testing if paging Enabled*/
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	serialPutStr("[INFO]: Paging is currently: ");
	if (cr0 & 0x80000000)
	    serialPutStr("ENABLED\r\n");
	else
	    serialPutStr("DISABLED\r\n");
	serialPutStr("[DEBUG]: END TESTING GDT\r\n");
	initKeboard(0);
}

void kernelInits(){
	initSerial();
	serialPutStr("[INFO]: SERIAL INIT SUCCESS\r\n");
	initDTs();
	gdtTest();
	serialPutStr("[INFO]: DISCRIPTOR TABLES INIT SUCCESS\r\n");
	initTty();
	serialPutStr("[INFO]: TTY INIT SUCCESS\r\n");
	init_timer(1000);	
	serialPutStr("[INFO]: TIMER INIT SUCCESS\r\n");
	setIRQHandler(1, keyboardHandler);
}

void	drawFace(){
	ttyAddStr("\r\n|-----|");	
	ttyAddStr("\r\n| . . |");	
	ttyAddStr("\r\n| ___ |");	
	ttyAddStr("\r\n|-----|\r\n\n");	
}

void	hi(){
	printfTty("say hi to 3amo\r\n");
}
void	exit(){
	printfTty("exiting in 3 .. ");
	fake_sleep(15000);
	printfTty("2 .. ");
	fake_sleep(15000);
	printfTty("1");
	fake_sleep(15000);
	printfTty("\r\nhaha d7akt 3lik you can't exit\r\n");
}
 

void	sound(uint32_t fr, uint32_t delay){
	play_sound(fr);
	msleep(delay);
	nosound();
}

 //Make a beep
 void beep(char *buff) {
	buff += 4;
	int32_t f = atoi(buff);
    uint32_t piano_keys[15] = {262, 294, 330, 349, 392,
							440, 494, 523, 587, 659, 
							697, 784, 880, 988, 1047};

	if (f != 0){
		printfTty("f = %d\r\n", f);
 		sound(f, 1000);
		return ;
	}
	for (size_t i = 0; i < 15; i++)
		sound(piano_keys[i], 500);

	printfTty("---- and check this ----\r\n");
	msleep(750);
	sound(800, 900);
	sound(700, 900);
	sound(600, 900);

	sound(800, 250);
	sound(700, 250);
	sound(600, 250);

	sound(800, 250);
	sound(700, 250);
	sound(600, 250);

	sound(800, 500);
	sound(700, 500);

	// sound(800, 250);
	// msleep(50);
	// sound(800, 250);
	// msleep(50);
	// sound(800, 250);
	// msleep(80);
	// sound(900, 250);
	// msleep(50);
	// sound(900, 250);
	// msleep(50);
	// sound(900, 250);
	// msleep(50);

 }

void kmain(){
	kernelInits();
	// printfTty("Hello my name is %s, nice to see all %u of You !%s\r\n", "Taha", 700, "!!");
	char *s;
	uint8_t clr = tty.color;
	changeTtyColor(VGA_YELLOW, -1);
	for (uint8_t i = 1; i < 5; i++)	// just to init the other Sessions as shells
	{
		switchSession(i);
		ttyAddStr("$> ");
	}
	tty.color = clr;
	switchSession(0);
	setDate(16, 5, 2024);
	shell();
}