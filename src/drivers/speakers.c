#include "ports.h"
#include "sound.h"
#include "timer.h"

/* R
	https://wiki.osdev.org/PC_Speaker
*/

// set sound using built-in speaker
void setSound(uint32_t frequency) {
	uint32_t div;
 	uint8_t tmp;
 
	if (!frequency) // carefull is zero play default beep
		frequency = 1000;
 	div = 1193180 / frequency; // set frequency
 	PbyteOut(0x43, 0xb6);
 	PbyteOut(0x42, (uint8_t) (div) );
 	PbyteOut(0x42, (uint8_t) (div >> 8));
 
    //And play the sound using the PC speaker
 	tmp = PbyteIn(0x61);
  	if (tmp != (tmp | 3))
 		PbyteOut(0x61, tmp | 3);
}

// time in ms
void	playSound(uint32_t fr, period_ms time){
	setSound(fr);
	msleep(time);
	nosound();
}

// shut up a sa7bi
void nosound() {

 	uint8_t tmp = PbyteIn(0x61) & 0xFC;
 
 	PbyteOut(0x61, tmp);
}