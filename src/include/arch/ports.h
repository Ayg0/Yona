#pragma once

#include "types.h"

void		ioWait(void);
uint8_t		pByteIn(uint16_t port_number);
uint16_t	pWordIn(uint16_t port_number);
void		pByteOut(uint16_t port_number, uint8_t value);
void		pWordOut(uint16_t port_number, uint16_t value);