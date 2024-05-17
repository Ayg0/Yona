#include "tty.h"
#include "str.h"
#include "timer.h"
#include "kLibStd.h"
#include "serialPorts.h"
#include "CPU/DiscriptorTables.h"
#include "keyboard.h"

_tty	tty;
uint32_t ticks;


char *validCmds[] = {
	"help",
	"clear",
	"add",
	"draw",
	"say",
	"hi",
	"exit",
	"settime",
	"setdate",
	"ps", // <- printStack
	NULL
};

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
	init_timer(50);	
	serialPutStr("[INFO]: TIMER INIT SUCCESS\r\n");
	setIRQHandler(1, keyboardHandler);
}

void	addNms(char *buff){
	int32_t	nb1, nb2;
	uint32_t	index = 0;
	buff += 3;
	nb1 = atoiS(buff, &index);
	buff += index;
	nb2 = atoiS(buff, &index);
	printfTty("%d + %d = %d\r\n", nb1, nb2, nb1 + nb2);
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

void parseDate(char *buff){
	int16_t d, mo;
	int32_t y;
	uint32_t index = 0;

	buff += 7;
	d = atoiS(buff, &index);
	buff += index;
	if (index > 3 || index == 0 || d > 31 || d < 1)
		goto MIS_USE;
	mo = atoiS(buff, &index);
	if (index > 3 || index == 0 || mo > 12 || mo < 1)
		goto MIS_USE;
	buff += index;
	y = atoiS(buff, &index);
	if (index > 5 || index == 0 || y < 0)
		goto MIS_USE;
	return (setDate(d, mo, y));

	MIS_USE:
	printfTty("MissUse: setdate day month year\r\n");
}

void parseTime(char *buff){
	int16_t s, m, h;
	uint32_t index = 0;

	buff += 7;
	h = atoiS(buff, &index);
	if (index > 3 || h > 60 || h < 0)
		goto MIS_USE;
	buff += index;
	m = atoiS(buff, &index);
	if (index > 3 || m > 60 || m < 0)
		goto MIS_USE;
	buff += index;
	s = atoiS(buff, &index);
	if (index > 3 || s > 60 || s < 0)
		goto MIS_USE;
	return(setTime(s, m, h));
	MIS_USE:
	printfTty("MissUse: settime hour min sec\r\n");
}

void	help(){
	printfTty("Welcome to YonaOs here is a list of all valid Commands: \r\n");
	
	for (uint32_t i = 0; validCmds[i]; i++)
		printfTty("-> %s\r\n", validCmds[i]);
}

void	printStack(){
	unsigned int esp;
	uint8_t c = 'a'; (void) c;

    asm volatile("mov %%esp, %0" : "=r" (esp));
    unsigned int stackBottom = ((unsigned int)&c);

    printfTty("Stack from top to bottom:\r\n");
    for (unsigned int* i = (unsigned int*)esp; i <= (unsigned int*)stackBottom; i++)
        printfTty("%x: %c\r\n", (void*)i, *i);
}

void (*functions[])(char *buff) = {
	help,
	clearTtySession,
	addNms,
	drawFace,
	say,
	hi,
	exit,
	parseTime,
	parseDate,
	printStack,
};


void	check_and_exec(char *buff){
	uint32_t i;

	for (i = 0; validCmds[i]; i++){
		if (!strncmp(buff, validCmds[i], strlen(validCmds[i])))
			return functions[i](buff);
	}
	printfTty("Command Not Found\r\n");
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
	while (1)
	{
		s = input("$> ");
		check_and_exec(s);
		clearBuffer();
	}
}