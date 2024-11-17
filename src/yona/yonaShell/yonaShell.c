# include "klibc/print.h"
# include "klibc/types.h"
# include "klibc/strings.h"
# include "drivers/keyboard.h"
# include "drivers/time.h"
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
    if (index >= MAX_COMMANDS) {
        S_ERR("Increase MAX_COMMANDS maybe ???", NULL);
        return;
    }
    commands[index].name = name;
    commands[index].func = func;
    index++;
}

void listCommands(char *args){
    (void)args;
    PRINT_K("Available commands:\n\r", NULL);
    for (uint8_t i = 0; commands[i].func; i++){
        if (commands[i].name && commands[i].func != listCommands){
            PRINT_K("->%s: ", commands[i].name);
            commands[i].func("--help");
        }
        msSleep(100);
    }
}

void initCommands(){
    initCommand("echo", echo);
    initCommand("peek", peek);
    initCommand("poke", poke);
    initCommand("hlt", hltCmd);
    initCommand("dump", dumpCmd);
    initCommand("reboot", reboot);
    initCommand("clear", clearTty);
    initCommand("stack", printStack);
    initCommand("help", listCommands);
    initCommand("snake", snakeGame);
}

uint8_t execCommand(char *command){
    char *name = strtok(command, " ");
    char *args = strtok(NULL, NULL);

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
		prompt("$", buffer);
        if (!(*buffer))
            continue;
		execCommand(buffer);
	}
}