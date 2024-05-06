#include "types.h"
// R https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access
// asm ( assembler template : output operands : input operands);

// Read a Byte from Port_number
uint8_t PbyteIn(uint16_t port_number){
	uint8_t result;
	/*
		reading from a port in assembly will be:
			mov dx, 0x03C4    ; set DX to the port number
			in al, dx        ; reading from port 0x03C4 and store in al
		so we do the same here except GNU uses AT&T syntax, which make the order of the operands reversed.
		also we are saying to store the value of al into result. and the the port number into dx
	*/
	__asm__ __volatile__ ("in %%dx, %%al" : "=a"(result) : "d"(port_number));
	return result ;
}
// Read a Word from Port_number
uint16_t PwordIn(uint16_t port_number){
	uint16_t result;
	__asm__ __volatile__ ("in %%dx, %%ax" : "=a"(result) : "d"(port_number));
	return result ;
}
// Write a Byte to Port_number
void	PbyteOut(uint16_t port_number, uint8_t value){
	/*
		writing to a port in assembly will be:
			mov dx, 0x03C4    ; set DX to the port number
			mov al, 0x45    ; set al with the value to print
			out dx, al        ; write to port 0x03C4 the value 0x45
	*/
	__asm__ __volatile__ ("out %%al, %%dx" : : "a"(value), "d"(port_number));
}
// Write a Word to Port_number
void	PwordOut(uint16_t port_number, uint16_t value){
	__asm__ __volatile__ ("out %%ax, %%dx" : : "a"(value), "d"(port_number));
}
/* Wait a very small amount of time (1 to 4 microseconds, generally).
Useful for implementing a small delay for PIC remapping on old hardware.*/
void ioWait(void){
    PbyteOut(0x80, 0);
}