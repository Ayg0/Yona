#include "types.h"
#include "klibc/vargs.h"
#include "klibc/print.h"

/* printf-ish Clone that takes in:
	- the function that print a character.
	- the format string.
	- and the vars.
*/

static int	putS(char *s, putCharFnc putChar){
	int i = 0;

	if (!s)
		return (putS("(NULL)", putChar));
	while(s[i]){
		putChar(s[i]);
		i++;
	}
	return i;
}

static int uPutNbr(size_t nbr, char *base, size_t baseLen, putCharFnc putChar){
	int printedSize = 0;

	if (nbr >= baseLen)
		printedSize = uPutNbr(nbr / baseLen, base, baseLen, putChar);
	putChar(base[nbr % baseLen]);
	return (++printedSize);
}

static int putNbr(int32_t nbr, putCharFnc putChar){
	int printedSize = 0;
	
	if (nbr < 0){
		printedSize = putChar('-');
		nbr = -nbr;
	}
	printedSize += uPutNbr(nbr, DEC_BASE, 10, putChar);

	return (printedSize);
}

int	handleFormatModifiers(varg_ptr *vptr, char *fmtString, int *i, putCharFnc putChar){
	int printedSize = 0;
	(void)vptr;

	switch (fmtString[*i])
	{
	case 's':
		printedSize = putS(VARG_NEXT(*vptr, char *), putChar);
		break;
	case 'u':
		printedSize = uPutNbr(VARG_NEXT(*vptr, uint32_t), DEC_BASE, 10, putChar);
		break;
	case 'd':
		printedSize = putNbr(VARG_NEXT(*vptr, int32_t), putChar);
		break;
	case 'x':
		printedSize = uPutNbr(VARG_NEXT(*vptr, uint32_t), HEX_BASE, 16, putChar);
		break;
	default:
		printedSize = putChar(fmtString[*i]);
		break;
	}

	return printedSize;
}

int	print(putCharFnc putChar, char *fmtString, ...){
	varg_ptr	vptr;
	int			i, printedSize;

	VARG_INIT(vptr, fmtString);
	i = 0;
	while (fmtString[i]) {
		if (fmtString[i] == '%'){
			i++;
			printedSize += handleFormatModifiers(&vptr, fmtString, &i, putChar);
		}
		else
			printedSize += putChar(fmtString[i]);
		i++;
	}
	return printedSize;
}