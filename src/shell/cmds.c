#include "str.h"
#include "tty.h"
#include "shell.h"
#include "timer.h"
#include "kLibStd.h"
#include "ports.h"
#include "sound.h"
#include "mem.h"

extern _tty tty;

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

void reboot() {
    uint8_t ready;
	do{
        ready = PbyteIn(0x64);
	} while (ready & 0x02);	// wait until keyboard is ready to accept a command.
    PbyteOut(0x64, 0xFE);	// send 0xFE command that causes the system to reboot.
    asm volatile ("hlt");
}

void	say(char *buff){
	buff += 3;
	while (*buff == ' ' || *buff == '\t')
		buff++;
	printfTty("Yona: %s\r\n", buff);
}

volatile uint32_t	ebp;
void printStack() {
    uint32_t	esp;
    uint8_t		i8[16] = {'a','b','c','d','e','f','g','h',
						'i','j','k','l','m','n','o','p'};
	volatile uint8_t	c = '~';
	((char *)&c)[-1] = 0;
	((char *)&c)[-2] = '~';
	((char *)&c)[-3] = 0;
	volatile uint16_t b = 'k'; (void)b;
	volatile uint32_t f = 0xABCF; (void)f;

    __asm__ __volatile__("mov %%esp, %0" : "=r" (esp));
	uint8_t termColor = tty.color;
	printfTty("          ------ ebp = %8x ; esp = %8x ------\r\n", ebp, esp);
    for (unsigned int* i = (unsigned int*)esp; i <= (unsigned int*)ebp; i += 4) {
        printfTty("%8x: ", i);

		memmove(i8, (uint8_t *)i, 16);
		for (int j = 0; j < 16; j++) {
			if (isPrintable(i8[j]))
                changeTtyColor(VGA_GREEN, -1);
			else if (i8[j] != 0)
                changeTtyColor(VGA_RED, -1);
            printfTty("%2p", i8[j]);
			if (j % 2)
				printfTty(" ");
			tty.color = termColor;
        }

        for (int j = 0; j < 16; j++) {
            if (isPrintable(i8[j])){
                changeTtyColor(VGA_GREEN, -1);
				printfTty("%c", i8[j]);
			}
            else if (i8[j] == 0)
                printfTty(".");
			else{
                changeTtyColor(VGA_RED, -1);
				printfTty(".");
			}
			tty.color = termColor;
        }
        printfTty("\r\n");
    }
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
 		playSound(f, 1000);
		return ;
	}
	for (size_t i = 0; i < 15; i++)
		playSound(piano_keys[i], 500);

	printfTty("---- and check this ----\r\n");
	msleep(750);
	playSound(800, 900);
	playSound(700, 900);
	playSound(600, 900);

	playSound(800, 250);
	playSound(700, 250);
	playSound(600, 250);

	playSound(800, 250);
	playSound(700, 250);
	playSound(600, 250);

	playSound(800, 500);
	playSound(700, 500);
}