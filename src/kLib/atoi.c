#include "types.h"
#include "str.h"
#include "kLibStd.h"
#include "serialio.h"
// safer atoi
int32_t atoiS(const char *s, uint32_t *index){
	int32_t		nbr;
	int8_t		sign = 1;

	*index = nbr = 0;
	while (s[*index] == ' ')
		*index += 1;
	if (s[*index] == '-' || s[*index] == '+'){
		sign += -2 * (s[*index] == '-');
		*index += 1;
	}
	while (s[*index])
	{
		if (!isDigit(s[*index]))
			break ;
		nbr *= 10;
		nbr += s[*index] - 48;
		*index += 1;
	}

	return nbr * sign;
}
// atoi
int32_t atoi(const char *s){
	int32_t		nbr;
	uint32_t	index;
	int8_t		sign = 1;

	index = nbr = 0;
	while (s[index] == ' ')
		index += 1;
	if (s[index] == '-' || s[index] == '+'){
		sign += -2 * (s[index] == '-');
		index += 1;
	}
	while (s[index])
	{
		if (!isDigit(s[index]))
			break ;
		nbr *= 10;
		nbr += s[index] - 48;
		index += 1;
	}
	return nbr * sign;
}

// uinsigned atoi
uint32_t	uatoiS(const char *s, uint32_t *index){
	uint32_t	nbr;

	*index = nbr = 0;
	while (s[*index] == ' ')
		*index += 1;
	while (s[*index])
	{
		if (!isDigit(s[*index]))
			break ;
		nbr *= 10;
		nbr += s[*index] - 48;
		*index += 1;
	}

	return nbr;
}

uint32_t	atoHexS(const char *s, uint32_t *index){
	uint32_t	nbr;
	uint32_t	fakeIndex;
	int			i;
	int			c;

	if (!index)
		index = &fakeIndex;
	*index = nbr = 0;
	while (s[*index] == ' ')
		*index += 1;
	if (s[*index] == '0' && s[*index + 1] == 'x')
		*index += 2;
	while (s[*index])
	{
		c = s[*index];
		c += (c >= 'A' && c <= 'Z') * 32;
		if ((i = strchrInd("0123456789abcdef", c)) == -1)
			break;
		nbr *= 16;
		nbr += i;
		*index += 1;
	}

	return nbr;
}