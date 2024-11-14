# include "klibc/print.h"
# include "klibc/types.h"
# include "klibc/strings.h"
# include "drivers/keyboard.h"
# include "yonaShell/yonaShell.h"
# include "drivers/vga/textMode/vgaTextMode.h"

_command commands[MAX_COMMANDS] = {0};

void handleCtrlC(){
	clearKeyboardBuffer();
	inturruptPrompting();
	S_INFO("Ctrl+C pressed\n\r", NULL);
}

void initCommand(char *name, commandFunc func){
    static uint8_t index = 0;
    if (index >= MAX_COMMANDS)
        return;
    commands[index].name = name;
    commands[index].func = func;
    index++;
}

void initCommands(){
    initCommand("clear", clearScreenBuffer);
    initCommand("echo", echo);
    initCommand("peek", peek);
    initCommand("poke", poke);
    initCommand("dump", dumpCmd);
    initCommand("reboot", reboot);
    initCommand("hlt", hltCmd);
    initCommand("stack", NULL);
    initCommand("help", NULL);
}

uint8_t execCommand(char *command){
    char *name = strtok(command, " ");
    char *args = name + strlen(name) + 1;

    for (uint8_t i = 0; i < MAX_COMMANDS; i++){
        if (!strcmp(commands[i].name, name)){
            if (commands[i].func)
                commands[i].func(args);
            S_SUCC("Command executed successfully\n", NULL);
            return 0;
        }
    }
    S_ERR("%s: Command not found\n", name);
    PRINT_K("%s: Command not found\r\n", name);
    return 1;
}

void initShell(){

	char buffer[256] = {0};

	setCtrlFunction(getScanCodeFromLetter('c'), handleCtrlC);
    initCommands();
	while (1){
		prompt("Yona", buffer);
        if (!(*buffer))
            continue;
		execCommand(buffer);
	}
}