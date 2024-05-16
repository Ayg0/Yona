#include "args.h"
#include "serialPorts.h"

static void	printSignedNbr(int32_t nbr){
	if (nbr < 0){
		serialPutChar('-');
		nbr *= -1;
	}
	serialPutNbr(nbr, 10, "0123456789");
}


static void	printNewArg(char identifier, varg_ptr *ptr){
	switch (identifier)
	{
		case 'c':	// print a character
			serialPutChar(VARG_NEXT(*ptr, char));
			break;
		case 's':	// print a string
			serialPutStr(VARG_NEXT(*ptr, char *));
			break;
		case 'u':	// print an unsigned int
			serialPutNbr(VARG_NEXT(*ptr, uint32_t), 10, "0123456789");
			break;
		case 'd':	// print an int
			printSignedNbr(VARG_NEXT(*ptr, int));
			break;
		case 'x':	// print unsigned int in Hex
			serialPutStr("0x");
			serialPutNbr(VARG_NEXT(*ptr, uint32_t), 16, "0123456789ABCDEF");
			break;
		default:
			serialPutChar(identifier);
	}
}
// %s %c %d %x %u Implemented ; modes: DEBUG, SUCC, ERR, INFO 
void	serialPrint(const char *FmtS, char *mode, ...){
	varg_ptr ptr;
	uint32_t i = 0;

	VARG_INIT(ptr, mode);
	serialPutStr(mode);
	while(FmtS[i]){
		if (FmtS[i] == '%'){
			i++, printNewArg(FmtS[i], &ptr);
		}
		else
			serialPutChar(FmtS[i]);
		i++;
	}
}