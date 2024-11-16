#pragma once

void initShell();

# define MAX_COMMANDS 16

typedef void (*commandFunc)(char *arg);

typedef struct command
{
    char		*name;
    commandFunc	func;
} _command;

// commands
void    poke(char *args);
void    echo(char *args);
void    peek(char *args);
void    reboot(char *args);
void    hltCmd(char *args);
void    dumpCmd(char *args);
void    clearTty(char *args);
void    printStack(char *args);