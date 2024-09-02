#pragma once
#include "drivers/vga/textMode/vgaTextMode.h"
#include "arch/serial.h"
typedef uint8_t (*putCharFnc)(char);

int	print(putCharFnc fnc, char *fmtString, ...);

// printf clone: Uses the VGA.
#define K_PRINT(FMT_STRING, ...) print(kPutC, FMT_STRING, __VA_ARGS__)
// printf clone: Uses the serial Communication.
#define S_PRINT(FMT_STRING, ...) print(SPutC, FMT_STRING, __VA_ARGS__)

#define S_INFO(FMT_STRING, ...)    print(SPutC, "\033[34mINFO\033[0m [%s] " FMT_STRING, __func__, __VA_ARGS__)
#define S_SUCC(FMT_STRING, ...)    print(SPutC, "\033[92mSUCC\033[0m [%s] " FMT_STRING, __func__, __VA_ARGS__)
#define S_ERR(FMT_STRING, ...)     print(SPutC, "\033[31mERR\033[0m [%s] " FMT_STRING, __func__, __VA_ARGS__)
#define S_DEBUG(FMT_STRING, ...)   print(SPutC, "\033[35mDEBUG\033[0m [%s] " FMT_STRING, __func__, __VA_ARGS__)