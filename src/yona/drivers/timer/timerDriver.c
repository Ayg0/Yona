#include "drivers/time.h"
#include "arch/i386/idts.h"
#include "klibc/print.h"

_sysClock date;
void	updateStatusBar();

void	tick(_registers r){
    static uint32_t ticks = 0;

    (void)r;
    date.msElapsedFromBoot++;
    ticks++;

    if (ticks != 1000)
        return;
    ticks = 0;
	date.s++;
	date.m  += (date.s == 60) && !(date.s = 0);
	date.h  += (date.m == 60) && !(date.m = 0);
	date.d  += (date.h == 24) && !(date.h = 0);
	date.mo += (date.d == 31) && !(date.d = 0);
	date.y  += (date.mo == 12) && !(date.mo = 0);
	updateStatusBar();
}


void msSleep(uint32_t ms){
	uint32_t start = date.msElapsedFromBoot;
	while (date.msElapsedFromBoot - start < ms);
}


void	initTimer(uint32_t frequency){
	uint16_t	divisor;

	divisor = 1193180 / frequency;
	date.frequency = frequency;
	// Configuration of the PIT to send the divisor to CHANNEL 0
	pByteOut(COMMAND_REG, CONFIGURE_PIT(PIT_CHANNEL0, PIT_LHB, MODE3, PIT_BINARY));
	// send the frequency divisor
	pByteOut(C0_DATA, divisor & 0xff);
	pByteOut(C0_DATA, (divisor >> 8) & 0xff);
	setIRQHandler(0, tick);
}

void setDate(uint8_t day, uint8_t month, uint32_t year){
	date.d = day;
	date.mo = month;
	date.y = year;
}

void setTime(uint8_t hour, uint8_t min, uint32_t sec){
	date.h = hour;
	date.m = min;
	date.s = sec;
}