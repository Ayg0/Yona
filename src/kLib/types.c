#include "types.h"

uint8_t	isAlpha(uint8_t c){
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

uint8_t	isPrintable(uint8_t c){
	return (c >= ' ' && c < 127);
}

uint8_t	isDigit(uint8_t c){
	return (c >= '0') && (c <= '9');
}