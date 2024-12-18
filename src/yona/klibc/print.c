#include "klibc/types.h"
#include "klibc/vargs.h"
#include "klibc/print.h"
#include "klibc/converts.h"

int16_t appendingWidth = 0;
char	appendingChar = ' ';
_buff	sprintBuff;

uint16_t appendBeforeNbr(size_t nbr, size_t baseLen, putCharFnc putChar){
	if (appendingWidth > 0){
		appendingWidth -= getNbrSize(nbr, baseLen);
		for (int16_t i = 0; i < appendingWidth; i++)
			putChar(appendingChar);
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
		printedSize += putChar('-');
	}
	printedSize += appendBeforeNbr(nbr, 10, putChar);
	printedSize += uPutNbr(nbr, DEC_BASE, 10, putChar);

	return (printedSize);
}

static int appendPutPtr(size_t ptr, char *base, size_t baseLen, putCharFnc putChar){
	int printedSize = 0;

	// appendingWidth -= 2;
	appendingChar = '0';
	printedSize += putChar('0');
	printedSize += putChar('x');
	printedSize += appendBeforeNbr(ptr, baseLen, putChar);
	printedSize += uPutNbr(ptr, base, baseLen, putChar);
	return printedSize;
}

int	handleFormatModifiers(varg_ptr *vptr, char *fmtString, int *i, putCharFnc putChar){
	int printedSize = 0;

	switch (fmtString[*i])
	{
	case 's':
		printedSize = putS(VARG_NEXT(*vptr, char *), putChar);
		break;
	case 'c':
		printedSize = putChar(VARG_NEXT(*vptr, int32_t));
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
	case 'b':
		printedSize = uAppendPutNbr(VARG_NEXT(*vptr, uint32_t), BIN_BASE, 2, putChar);
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


// implemented %c, %s, %u, %d, %x, %p, %b, appending with ' ' and 0
int	print(putCharFnc putChar, char *fmtString, ...){
	varg_ptr	vptr;
	int			i, printedSize;

	VARG_INIT(vptr, fmtString);
	i = 0;
	while (fmtString[i]) {
		// reseting the printing modifiers;
		appendingWidth = 0;
		appendingChar = ' ';
		// looking for the next printing specifier;
		if (fmtString[i] == '%'){
			i++;
			// example: %03d => so we append with Zeros instead of spaces;
			if (fmtString[i] == '0')
				appendingChar = '0', i++;
			// getting the appending value if there is any
			if (isDigit(fmtString[i]))
				appendingWidth = getApendingNbr(fmtString, &i);
			// printing the variable;
			printedSize += handleFormatModifiers(&vptr, fmtString, &i, putChar);
		}
		else
			printedSize += putChar(fmtString[i]);
		i++;
	}
	return printedSize;
}

uint8_t appendsprintBuff(char c){
	if (!sprintBuff.buff)
		S_ERR("sprintBuff is not set properly !!\r\n", NULL);
	sprintBuff.buff[sprintBuff.index] = c;
	sprintBuff.index++;
	return 1;
}

// used to set/reset the sprintBuff:
void setSprintBuff(char *buff){
	if (sprintBuff.buff)
		sprintBuff.buff[sprintBuff.index] = 0;
	sprintBuff.buff = buff;
	sprintBuff.index = 0;
}
