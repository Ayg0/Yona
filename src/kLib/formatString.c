#include "args.h"
#include "serialio.h"
#include "kLibStd.h"

char	HEX_BASE[] = "0123456789ABCDEF";
char	DEC_BASE[] = "0123456789";
char	BINARY_BASE[] = "01";

uint32_t appendLen = 0;

void	appendChar(char **string, uint32_t *index, char c){
	(*string)[*index] = c;
	(*index)++;
}

void	appendString(char **string, uint32_t *index, char *toAppend){
	if (!toAppend)
		return ;
	while (*toAppend)
	{
		appendChar(string, index, *toAppend);
		toAppend++;
	}
}

void	appendUnsignNbr(char **string, uint32_t *index, uint32_t nbr){
	uint32_t i = 0;
	uint32_t nbrSize = getNbrSize(nbr, 10);

	if (nbrSize < appendLen)
		nbrSize += appendLen - nbrSize;
	while (i < nbrSize){
		(*string)[*index + nbrSize - i - 1] = DEC_BASE[nbr % 10];
		nbr /= 10, i++;
	}
	*index += i;
}

void	appendHex(char **string, uint32_t *index, uint32_t nbr){
	uint32_t i = 0;
	uint32_t nbrSize = getNbrSize(nbr, 16);

	appendString(string, index, "0x");
	if (nbrSize < appendLen)
		nbrSize += appendLen - nbrSize;
	while (i < nbrSize){
		(*string)[*index + nbrSize - i - 1] = HEX_BASE[nbr % 16];
		nbr /= 16, i++;
	}
	*index += i;
}

void	appendSignNbr(char **string, uint32_t *index, int32_t nbr){
	if (nbr < 0){
		appendChar(string, index, '-');
		nbr *= -1;
	}
	appendUnsignNbr(string, index, nbr);
}

void appendArg(char identifier, char **str, uint32_t *index, varg_ptr *ptr){
switch (identifier)
	{
		case 'c':	// append a character
			appendChar(str, index, VARG_NEXT(*ptr, char));
			break;
		case 's':	// append a string
			appendString(str, index, VARG_NEXT(*ptr, char *));
			break;
		case 'u':	// append an unsigned int
			appendUnsignNbr(str, index, VARG_NEXT(*ptr, uint32_t));
			break;
		case 'd':	// append an int
			appendSignNbr(str, index, VARG_NEXT(*ptr, int));
			break;
		case 'x':	// append an int
			appendHex(str, index, VARG_NEXT(*ptr, uint32_t));
			break;
		default:
			appendChar(str, index, identifier);
	}
}
uint32_t	increaseAppendLen(char *fmtS){
	uint32_t	i = 0;

	appendLen = atoiS(fmtS, &i);
	return i;
}

void	formatString(char *fmtS, char **str, ...){
	varg_ptr	ptr;
	uint32_t	index = 0;
	
	VARG_INIT(ptr, str);
	while (*fmtS)
	{
		appendLen = 0;
		if (*fmtS == '%'){
			fmtS++;
			if (*fmtS >= '0' && *fmtS <= '9')
				fmtS += increaseAppendLen(fmtS);
			appendArg(*fmtS, str, &index, &ptr);
		}
		else
			appendChar(str, &index, *fmtS);
		if (*fmtS == 0)
			break;
		fmtS++;
	}
	(*str)[index] = '\0';
}