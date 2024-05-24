#pragma once

#include "types.h"

// cmds
void	hlt();
void	reboot();
void	printStack();
void	say(char *buff);
void	help(char *buff);
void	peek(char *buff);
void	poke(char *buff);
void	beep(char *buff);
void	parseDate(char *buff);
void	parseTime(char *buff);