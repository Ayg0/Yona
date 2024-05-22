#include "types.h"
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