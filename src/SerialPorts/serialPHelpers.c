#include "ports.h"
/* R
	https://wiki.osdev.org/Serial_Ports
	https://www.lammertbies.nl/comm/info/serial-uart
	if you want to learn more about serial Communication: https://www.youtube.com/watch?v=IyGwvGzrqp8
	more about the baud rate: https://www.wevolver.com/article/baud-rates
*/

#define PORT 0x3f8   /* COM1 */

// init the serial communication to use the UART Protocol
int initSerial() {
	// Disable all interrupts (SERIAL Interupts), making it so you need  to check if there is an input
   PbyteOut(PORT + 1, 0x00);
   // Enable DLAB (set baud rate divisor), enables you to change the rate
   PbyteOut(PORT + 3, 0x80);
   PbyteOut(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   PbyteOut(PORT + 1, 0x00);    //                  (hi byte)
   PbyteOut(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   PbyteOut(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   PbyteOut(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   PbyteOut(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   PbyteOut(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(PbyteIn(PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   PbyteOut(PORT + 4, 0x0F);
   return 0;
}

int isTransmitEmpty() {
   return PbyteIn(PORT + 5) & 0x20;
}

void serialPutChar(char a) {
   while (isTransmitEmpty() == 0);
   PbyteOut(PORT, a);
}

void	serialPutNbr(uint32_t a, uint32_t len, char *base)
{
	if (a >= len)
		serialPutNbr((a / len), len, base);
	serialPutChar(base[a % len]);
}

void serialPutStr(char* str) {
	if (!str)
		return serialPutStr("(NULL)");
    for(size_t i = 0; str[i]; i++) {
        serialPutChar(str[i]);
    }
}

int isSerialReceived() {
   return PbyteIn(PORT + 5) & 1;
}

char serialRead() {
   while (isSerialReceived() == 0);
 
   return PbyteIn(PORT);
}