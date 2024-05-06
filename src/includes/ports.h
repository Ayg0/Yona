#pragma once
#include "types.h"

uint8_t		PbyteIn(uint16_t port_number);
uint16_t	PwordIn(uint16_t port_number);
void		PbyteOut(uint16_t port_number, uint8_t value);
void		PwordOut(uint16_t port_number, uint16_t value);
void 		ioWait(void);