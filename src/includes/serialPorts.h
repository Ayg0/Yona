#pragma once
#include "types.h"

#define ERR		"[ERROR]: "
#define SUCC	"[SUCCESS]: "
#define INFO	"[INFO]: "
#define DEBUG 	"[DEBUG]: "


int		initSerial();
char	serialRead();
int		isTransmitEmpty();
int		isSerialReceived();
void	serialPutChar(char a);
void	serialPutStr(char* str);
void	serialPutNbr(uint32_t a, uint32_t len, char *base);
void	serialPrint(const char *FmtS, char *mode, ...);

#define SERIAL_ERR(FMT, ...) (serialPrint(FMT, ERR, __VA_ARGS__));
#define SERIAL_INFO(FMT, ...) (serialPrint(FMT, INFO, __VA_ARGS__));
#define SERIAL_SUCC(FMT, ...) (serialPrint(FMT, SUCC, __VA_ARGS__));
#define SERIAL_DEBUG(FMT, ...) (serialPrint(FMT, DEBUG, __VA_ARGS__));