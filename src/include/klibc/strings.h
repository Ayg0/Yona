#pragma once

#include "types.h"

size_t  strlen(const char *str);
size_t  strlcpy(char *dest, const char *src, size_t size);
size_t  strlcat(char *dest, const char *src, size_t size);
int32_t strchrInd(const char *s, int c);
char    *strchr(const char *s, int c);
char    *strrchr(const char *s, int c);
int     strncmp(const char *s1, const char *s2, size_t n);
int     strcmp(const char *s1, const char *s2);
char    *strnstr(const char *big, const char *little, size_t len);
