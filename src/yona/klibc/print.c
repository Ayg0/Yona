#include "types.h"
#include "klibc/vargs.h"
#include "klibc/print.h"
#include "klibc/converts.h"

/* printf-ish Clone that takes in:
	- the function that print a character.
	- the format string.
	- and the vars.
*/

int16_t appendingWidth = 0;

uint16_t appendBeforeNbr(size_t nbr, size_t baseLen, putCharFnc putChar){
	if (appendingWidth > 0){
		appendingWidth -= getNbrSize(nbr, baseLen);
		for (int16_t i = 0; i < appendingWidth; i++)
			putChar(' ');
	}
	return appendingWidth;
}

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

static int uAppendPutNbr(size_t nbr, char *base, size_t baseLen, putCharFnc putChar){
	int printedSize = 0;

	printedSize += appendBeforeNbr(nbr, baseLen, putChar);
	printedSize += uPutNbr(nbr, base, baseLen, putChar);
	return printedSize;
}

static int putNbr(int32_t nbr, putCharFnc putChar){
	int printedSize = 0;

	if (nbr < 0){
		nbr = -nbr;
		appendingWidth--;
		printedSize += appendBeforeNbr(nbr, 10, putChar);
		printedSize += putChar('-');
	}
	printedSize += uPutNbr(nbr, DEC_BASE, 10, putChar);

	return (printedSize);
}

static int appendPutPtr(size_t ptr, char *base, size_t baseLen, putCharFnc putChar){
	int printedSize = 0;

	appendingWidth -= 2;
	printedSize += appendBeforeNbr(ptr, baseLen, putChar);
	printedSize += putChar('0');
	printedSize += putChar('x');
	printedSize += uPutNbr(ptr, base, baseLen, putChar);
	return printedSize;
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
		printedSize = uAppendPutNbr(VARG_NEXT(*vptr, uint32_t), DEC_BASE, 10, putChar);
		break;
	case 'd':
		printedSize = putNbr(VARG_NEXT(*vptr, int32_t), putChar);
		break;
	case 'x':
		printedSize = uAppendPutNbr(VARG_NEXT(*vptr, uint32_t), HEX_BASE, 16, putChar);
		break;
	case 'p':
		printedSize = appendPutPtr(VARG_NEXT(*vptr, size_t), HEX_BASE, 16, putChar);
		break;
	default:
		printedSize = putChar(fmtString[*i]);
		break;
	}
	return printedSize;
}

int16_t	getApendingNbr(char *fmtString, int *i){
	int16_t nbr = 0;
	uint32_t	index;

	nbr = atoiS(fmtString + *i, &index);
	*i += index;

	return nbr;
}

// implemented %s, %u, %d, %x
int	print(putCharFnc putChar, char *fmtString, ...){
	varg_ptr	vptr;
	int			i, printedSize;

	VARG_INIT(vptr, fmtString);
	i = 0;
	while (fmtString[i]) {
		appendingWidth = 0;
		if (fmtString[i] == '%'){
			i++;
			if (isDigit(fmtString[i]))
				appendingWidth = getApendingNbr(fmtString, &i);
			printedSize += handleFormatModifiers(&vptr, fmtString, &i, putChar);
		}
		else
			printedSize += putChar(fmtString[i]);
		i++;
	}
	return printedSize;
}