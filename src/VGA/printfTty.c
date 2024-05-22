#include "tty.h"
#include "args.h"
#include "kLibStd.h"
#include "serialio.h"

static int32_t appendLen = 0;

static void	printSignedNbr(int32_t nbr){
	if (nbr < 0){
		ttyAddChar('-');
		nbr *= -1;
	}
	ttyAddNbr(nbr, 10, "0123456789");
}

// base = NULL will result in printing a signed decimal 32 bit
void	printNbrAppend(uint32_t nbr, char *base, uint8_t baseLen){
	baseLen = baseLen == 0 ? 10: baseLen;
	uint32_t nbrSize = getNbrSize(nbr, baseLen);

	appendLen -= nbrSize;
	while (appendLen > 0)
		ttyAddChar('0'), appendLen--;
	if (!base)
		printSignedNbr(nbr);
	else
		ttyAddNbr(nbr, baseLen, base);
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
			printNbrAppend(VARG_NEXT(*ptr, uint32_t), DEC_BASE, 10);
			break;
		case 'd':	// print an int
			printNbrAppend(VARG_NEXT(*ptr, int32_t), NULL, 0);
			break;
		case 'x':	// print unsigned int in Hex
			ttyAddStr("0x");
			printNbrAppend(VARG_NEXT(*ptr, uint32_t), HEX_BASE, 16);
			break;
		default:
			ttyAddChar(identifier);
	}
}

uint32_t increaseLen(const char *appendingFmt){
	uint32_t	i = 0;

	appendLen = atoiS(appendingFmt, &i);
	return i;
}
// %s %c %d %x %u Implemented, %4x => append the number until 4 digits
void	printfTty(const char *FmtS, ...){
	varg_ptr ptr;
	uint32_t i = 0;

	VARG_INIT(ptr, FmtS);
	while(FmtS[i]){
		appendLen = 0;
		if (FmtS[i] == '%'){
			i++;
			if (isDigit(FmtS[i]))
				i += increaseLen(&FmtS[i]);
			printNewArg(FmtS[i], &ptr);
		}
		else
			ttyAddChar(FmtS[i]);
		i++;
	}
}