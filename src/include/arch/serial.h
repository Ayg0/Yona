#pragma once
#include "klibc/types.h"
#include "arch/ports.h"

#define COM1 0x3F8

int			initSerial();
int			isTransmitEmpty();

uint8_t SPutC(char c);