#pragma once
#include "drivers/vga/textMode/vgaTextMode.h"
#include "arch/serial.h"
typedef uint8_t (*putCharFnc)(char);

int	print(putCharFnc fnc, char *fmtString, ...);

// printf clone: Uses the VGA.
#define K_PRINT(FMT_STRING, ...) print(kPutC, FMT_STRING, __VA_ARGS__)
// printf clone: Uses the serial Communication.
#define S_PRINT(FMT_STRING, ...) print(SPutC, FMT_STRING, __VA_ARGS__)