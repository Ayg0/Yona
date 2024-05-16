#include "tty.h"
#include "str.h"
#include "timer.h"
#include "kLibStd.h"
#include "serialPorts.h"
#include "CPU/DiscriptorTables.h"
#include "keyboard.h"

_tty	mainTty;
uint32_t ticks;

extern gdtEntry	gdtEntries[GDT_ENTRIES];

// void fake_sleep(uint32_t iters) __attribute__((deprecated));

void fake_sleep(uint32_t iters){
	for (size_t volatile i = 0; i < iters; i++)
		for (size_t j = 0; j < iters; j++)
			i = i;
}

void gdtTest() {
    char*		addr;
	uint32_t	cr0;

	serialPutStr("[DEBUG]: TESTING GDT\r\n");
    /* Test data segment */
    asm volatile("mov %%ds, %0" : "=r" (addr));
    serialPutStr("Data Segment Base Address: ");
	serialPutNbr((uint32_t) addr, 10, "0123456789");
	serialPutStr("\r\n");

    /* Test code segment */
    asm volatile("mov %%cs, %0" : "=r" (addr));
    serialPutStr("Code Segment Base Address: ");
	serialPutNbr((uint32_t) addr, 10, "0123456789");
	serialPutStr("\r\n");
    /* Test stack segment */
    asm volatile("mov %%ss, %0" : "=r" (addr));
    serialPutStr("Stack Segment Base Address: ");
	serialPutNbr((uint32_t) addr, 10, "0123456789");
	serialPutStr("\r\n");
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
	init_timer(50);	
	serialPutStr("[INFO]: TIMER INIT SUCCESS\r\n");
	setIRQHandler(1, keyboardHandler);
}

void	addNms(char *buff){
	(void)buff;
	printfTty("ok let's start with 1 + 1\r\n1 + 1 = 3 ?\r\naaaa didn't learn addition yet !!\r\n");
}

void	drawFace(){
	ttyAddStr("\r\n|-----|");	
	ttyAddStr("\r\n| . . |");	
	ttyAddStr("\r\n| ___ |");	
	ttyAddStr("\r\n|-----|\r\n\n");	
}

void	say(char *buff){
	buff += 3;
	while (*buff == ' ' || *buff == '\t')
		buff++;
	printfTty("Yona: %s\r\n", buff);
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

char *valid_cmds[] = {
	"clear",
	"add",
	"draw",
	"say",
	"hi",
	"exit",
	NULL
};
void (*functions[])(char *buff) = {
	clearTtySession,
	addNms,
	drawFace,
	say,
	hi,
	exit,
};

void	check_and_exec(char *buff){
	uint32_t i;

	for (i = 0; valid_cmds[i]; i++){
		if (!strncmp(buff, valid_cmds[i], strlen(valid_cmds[i])))
			return functions[i](buff);
	}
	printfTty("Command Not Found\r\n");
}


void	hanldeCommand(char *s){
	if (!strcmp(s, "hi"))
		printfTty("Say hi to 3amo\r\n");
	else if (!strncmp(s, "say ", 4))
		printfTty("Yona: %s\r\n", s + 4);
	else
		printfTty("Command Not Found\r\n");
}

void kmain(){
	kernelInits();
	// printfTty("Hello my name is %s, nice to see all %u of You !%s\r\n", "Taha", 700, "!!");
	char *s;
	for (uint8_t i = 1; i < 5; i++)	// just to init the other Sessions as shells
	{
		switchSession(i);
		ttyAddStr("$> ");
	}
	switchSession(0);
	setDate(16, 5, 2024);
	while (1)
	{
		s = input("$> ");
		// hanldeCommand(s);
		check_and_exec(s);
		clearBuffer();
	}
	
}