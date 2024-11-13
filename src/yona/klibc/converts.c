#include "klibc/types.h"
#include "klibc/strings.h"

static uint32_t skipSpaces(const char *s){
	uint32_t index;

	index = 0;
	while (s[index] == ' ')
		index++;
	return index;
}

// safer atoi
int32_t atoiS(const char *s, uint32_t *index){
	int32_t		nbr = 0;
	int8_t		sign = 1;
	uint32_t	fakeIndex;

	if (!index)	// if index is NULL, we use a fake index
		index = &fakeIndex;

	*index = skipSpaces(s);
	if (s[*index] == '-' || s[*index] == '+'){
		sign += (s[*index] == '-') ? -1 : 1;
		(*index)++;
	}

	while (isDigit(s[*index]))
	{
		nbr = nbr * 10 + (s[*index] - 48);
		(*index)++;
	}

	return nbr * sign;
}

// uinsigned atoi
uint32_t	uatoiS(const char *s, uint32_t *index){
	uint32_t	nbr;
	uint32_t	fakeIndex;

	if (!index)	// if index is NULL, we use a fake index
		index = &fakeIndex;

	*index = nbr = 0;
	// skip spaces
	*index = skipSpaces(s);
	while (isDigit(s[*index]))
	{
		nbr = nbr * 10 + (s[*index] - 48);
		(*index)++;
	}

	return nbr;
}

uint32_t	atoHexS(const char *s, uint32_t *index){
	uint32_t	nbr = 0;
	uint32_t	fakeIndex;
	int			i;
	int			c;

	if (!index)
		index = &fakeIndex;

	// skip spaces
	*index = skipSpaces(s);

	// skip 0x if present
	if (s[*index] == '0' && s[*index + 1] == 'x')
		*index += 2;

	while (s[*index])
	{
		c = s[*index];
		c -= (c >= 'a' && c <= 'z') * 32; // convert to UpperCase
		i = strchrInd(HEX_BASE, c);
		if (i == -1)
			break;
		nbr = (nbr * 16) + i; // update number
		*index += 1;
	}
	return nbr;
}