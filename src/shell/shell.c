#include "tty.h"
#include "str.h"
#include "shell.h"

char *validCmds[] = {
	"help",
	"clear",
	"say",
	"settime",
	"setdate",
	"stack",
	"reboot",
	"hlt",
	"beep",
	"peek",
	"poke",
	"dump",
	NULL
};

char *desciption[] = {
	"Shows good info for you, yes yes!!",
	"clear current Session",
	"say is just echo",
	"set time for Yona, no params = reset",
	"sets the date wow",
	"print the stack from top to buttom",
	"self explanatory",
	"zam takram ala mitaaasyon jmaad",
	"1sec of your favourite frequency, no params = music",
	"peek addr -> read a byte from addr",
	"poke addr byte -> write the byte to addr",
	"dump start size -> dump memory from start to start + size",
	NULL
};


void (*f[])(char *buff) = {
	help,
	clearTtySession,
	say,
	parseTime,
	parseDate,
	printStack,
	reboot,
	hlt,
	beep,
	peek,
	poke,
	shellDump,
};

void hlt(){
	printfTty("I ain't doing a thing until u reboot the pc !!");
	__asm__ volatile("cli; hlt");
}

void	printOverView(){
	printfTty("Welcome to YonaOs here is a list of all valid Commands: \r\n");
	for (uint32_t i = 0; validCmds[i]; i++)
		printfTty("- %s\r\n", validCmds[i]);
}

void	help(char *buff){
	buff += 4;

	while (*buff == ' ')
		buff++;
	if (!*buff)
		return printOverView();

	for (uint32_t i = 0; validCmds[i]; i++){
		if (!strcmp(buff, validCmds[i]))
			return printfTty("-> %s: %s\r\n", validCmds[i], desciption[i]);
	}
	printfTty("`%s` ?? I have no clue either\r\n", buff);	
}

void	checkAndExec(char *buff){
	uint32_t i;

	for (i = 0; validCmds[i] != NULL; i++){
		if (!strncmp(buff, validCmds[i], strlen(validCmds[i])))
			return f[i](buff);
	}
	printfTty("Command Not Found\r\n");
}

void	shell(){
	char	*s;

	while (1){
		s = input("$> ");
		checkAndExec(s);
		clearBuffer();
	}
}
