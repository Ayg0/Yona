#pragma once
#include "types.h"

int		initSerial();
int		isTransmitEmpty();
void	serialPutChar(char a);
void	serialPutStr(char* str);
void	serialPutNbr(uint32_t a, uint32_t len, char *base);
int		isSerialReceived();
char	serialRead();