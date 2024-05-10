#pragma once

int		initSerial();
int		isTransmitEmpty();
void	serialPutChar(char a);
void	serialPutStr(char* str);
void	serialPutNbr(unsigned long a, unsigned long len, char *s);
int		isSerialReceived();
char	serialRead();