#include "tty.h"
#include "args.h"
#include "serialio.h"
static void	printSignedNbr(int32_t nbr){
	serialPutNbr(nbr, 10, "0123456789");
	if (nbr < 0){
		ttyAddChar('-');
		nbr *= -1;
	}
	ttyAddNbr(nbr, 10, "0123456789");
}

static void	printString(char *s){
	if (s)
		return (ttyAddStr(s));
	ttyAddStr("(NULL)");
}


void	printNewArg(char identifier, varg_ptr *ptr){
	switch (identifier)
	{
		case 'c':	// print a character
			ttyAddChar(VARG_NEXT(*ptr, char));
			break;
		case 's':	// print a string
			printString(VARG_NEXT(*ptr, char *));
			break;
		case 'u':	// print an unsigned int
			ttyAddNbr(VARG_NEXT(*ptr, uint32_t), 10, "0123456789");
			break;
		case 'd':	// print an int
			printSignedNbr(VARG_NEXT(*ptr, int));
			break;
		case 'x':	// print unsigned int in Hex
			ttyAddStr("0x");
			ttyAddNbr(VARG_NEXT(*ptr, uint32_t), 16, "0123456789ABCDEF");
			break;
		default:
			ttyAddChar(identifier);
	}
}
// %s %c %d %x %u Implemented
void	printfTty(const char *FmtS, ...){
	varg_ptr ptr;
	uint32_t i = 0;

	VARG_INIT(ptr, FmtS);
	while(FmtS[i]){
		if (FmtS[i] == '%'){
			i++, printNewArg(FmtS[i], &ptr);
		}
		else
			ttyAddChar(FmtS[i]);
		i++;
	}
}