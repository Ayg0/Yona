#pragma once


void	formatString(char *fmtS, char **str, ...);

int32_t		atoi(const char *s);
int32_t		atoiS(const char *s, uint32_t *index);
uint32_t	uatoiS(const char *s, uint32_t *index);
uint32_t	atoHexS(const char *s, uint32_t *index);