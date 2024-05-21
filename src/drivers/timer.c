#include "tty.h"
#include "timer.h"
#include "CPU/DiscriptorTables.h"
#include "serialio.h"
#include "vga.h"

uint32_t ticks = 0;
volatile _time date;
extern _tty tty;
uint8_t direction;

void	tick(registers Rs){
	(void)Rs;
	ticks++;
	date.msFromPcStart++;
	if (ticks < date.frequency){
		if (!(ticks % (date.frequency / 3))){
			direction = !direction;
			updateStatusBar();
		}
		return;
	}
	ticks = 0;
	date.s++;
	date.m += (date.s == 60) && !(date.s = 0);
	date.h += (date.m == 60) && !(date.m = 0);
	date.d += (date.h == 24) && !(date.h = 0);
	date.mo += (date.d == 31) && !(date.d = 0);
	date.y += (date.mo == 12) && !(date.mo = 0);
	updateStatusBar();
}


void msleep(uint32_t mSeconds) {
	uint32_t next = date.msFromPcStart + mSeconds;

	while (date.msFromPcStart < next){
	}
}

_time	getDate(){
	return date;
}

void	setTime(uint8_t s, uint8_t m, uint8_t h){
	date.s = s;
	date.m = m;
	date.h = h;
}

void	setDate(uint8_t d, uint8_t mo, uint32_t y){
	date.d = d;
	date.mo = mo;
	date.y = y;
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