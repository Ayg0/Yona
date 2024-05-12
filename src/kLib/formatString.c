#include "args.h"
#include "serialPorts.h"

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

uint32_t	getNbrSize(uint32_t nbr){
	uint32_t i = 1;
	while (nbr)
		nbr /= 10, i++;
	return i;	
}

void	appendUnsignNbr(char **string, uint32_t *index, uint32_t nbr){
	uint32_t nbrSize = getNbrSize(nbr);
	uint32_t i = 0;
	while (i < nbrSize)
	{
		(*string)[*index + nbrSize - i - 1] = nbr % 10 + 48;
		nbr /= 10, i++;
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
		// case 'x':	// print unsigned int in Hex
		// 	ttyAddStr("0x");
		// 	ttyAddNbr(VARG_NEXT(*ptr, uint32_t), 16, "0123456789ABCDEF");
		// 	break;
		default:
			appendChar(str, index, identifier);
	}
}

void	formatString(char *fmtS, char **str, ...){
	varg_ptr	ptr;
	uint32_t	index = 0;

	VARG_INIT(ptr, str);
	while (*fmtS)
	{
		if (*fmtS == '%')
			fmtS++, appendArg(*fmtS, str, &index, &ptr);
		else
			appendChar(str, &index, *fmtS);
		fmtS++;
	}
	// str[index++] = '\0';
}