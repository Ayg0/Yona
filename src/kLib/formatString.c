#include "args.h"
#include "serialPorts.h"


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

uint32_t	getNbrSize(uint32_t nbr){
	uint32_t i = 0;

	do{
		nbr /= 10, i++;
	}while (nbr);

	return i;	
}

void	appendUnsignNbr(char **string, uint32_t *index, uint32_t nbr){
	uint32_t i = 0;
	uint32_t nbrSize = getNbrSize(nbr);

	if (nbrSize < appendLen)
		nbrSize += appendLen - nbrSize;
	while (i < nbrSize){
		(*string)[*index + nbrSize - i - 1] = (nbr % 10) + 48;
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
		default:
			appendChar(str, index, identifier);
	}
}
uint32_t	increaseAppendLen(char *fmtS){
	uint32_t	i = 0;
	uint32_t	len = 0;

	while (fmtS[i] >= '0' && fmtS[i] <= '9')
	{
		len *= 10;
		len += (fmtS[i] - 48);
		i++;
	}
	appendLen = len;
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