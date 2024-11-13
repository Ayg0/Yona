#include "klibc/types.h"

uint8_t	isAlpha(uint8_t c){
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

uint8_t	isPrintable(uint8_t c){
	return (c >= ' ' && c < 127);
}

uint8_t	isLowerCase(uint8_t c){
	return (c >= 'a' && c <= 'z');
}

uint8_t	isUpperCase(uint8_t c){
	return (c >= 'A' && c <= 'Z');
}

uint8_t	isDigit(uint8_t c){
	return (c >= '0') && (c <= '9');
}

uint32_t	getNbrSize(uint32_t nbr, uint8_t baseLen){
	uint32_t i = 0;

	do{
		nbr /= baseLen, i++;
	}while (nbr);

	return i;	
}