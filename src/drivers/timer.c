#include "tty.h"
#include "timer.h"
#include "CPU/DiscriptorTables.h"
#include "serialPorts.h"
uint32_t ticks = 0;
static _time date;

void	tick(registers Rs){
	(void)Rs;
	ticks++;
	if (ticks < date.frequency)
		return;
	ticks = 0;
	date.s++;
	date.m += (date.s == 60) && !(date.s = 0);
	date.h += (date.m == 60) && !(date.m = 0);
	date.d += (date.h == 24) && !(date.h = 0);
	date.mo += (date.d == 31) && !(date.d = 0);
	date.year += (date.mo == 12) && !(date.mo = 0);
	updateStatusBar();
}

_time	getDate(){
	return date;
}

void	init_timer(uint32_t frequency){
	uint16_t	divisor;

	divisor = 1193180 / frequency;
	date.frequency = frequency;
	// Configuration of the PIT to send the divisor to CHANNEL 0
	PbyteOut(COMMAND_REG, CONFIGURE_PIT(PIT_CHANNEL0, PIT_LHB, MODE3, PIT_BINARY));
	// send the frequency divisor
	PbyteOut(C0_DATA, divisor & 0xff);
	PbyteOut(C0_DATA, (divisor >> 8) & 0xff);
	setIRQHandler(0, tick);
}