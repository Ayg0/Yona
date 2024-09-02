#include "arch/serial.h"

#define COM1 0x3f8   /* COM1 */

// init the serial communication to use the UART Protocol
int initSerial() {
	// Disable all interrupts (SERIAL Interupts), making it so you need  to check if there is an input
   pByteOut(COM1 + 1, 0x00);
   // Enable DLAB (set baud rate divisor), enables you to change the rate
   pByteOut(COM1 + 3, 0x80);
   pByteOut(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   pByteOut(COM1 + 1, 0x00);    //                  (hi byte)
   pByteOut(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
   pByteOut(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   pByteOut(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   pByteOut(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
   pByteOut(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(pByteIn(COM1 + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   pByteOut(COM1 + 4, 0x0F);
   return 0;
}

// checks if serial communication ready for data transmition
int isTransmitEmpty() {
   return pByteIn(COM1 + 5) & 0x20;
}

// putchar for serial Communication
uint8_t SPutC(char c){
   while (isTransmitEmpty() == 0) {}

	pByteOut(COM1, c);
   return 1;
}