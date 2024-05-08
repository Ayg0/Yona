#pragma once

int		initSerial();
int		isTransmitEmpty();
void	serialPutChar(char a);
void	serialPutStr(char* str);
int		isSerialReceived();
char	serialRead();