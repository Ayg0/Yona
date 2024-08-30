#include "types.h"
#include "klibc/memory.h"
size_t strlen(const char *str){
    size_t i = 0;

    while (str[i])
        i++;
    return i;
}
//  strlcpy
size_t	strlcpy(char *dest, const char *src, size_t size){
    while (size > 0 && *src)
    {
        *dest = *src;
        dest++, src++;
        size--;
    }
    *dest = 0;
    return strlen(src);
}
//  strlcat
size_t	strlcat(char *dest, const char *src, size_t size){
    size_t destLen = strlen(dest);
    size_t srcLen = strlen(src);
    size_t copyLen;

    if (destLen + 1 >= size) // Not enough space in dst
        return destLen + srcLen;

    copyLen = size - destLen - 1;
    if (copyLen > srcLen)
        copyLen = srcLen;

    memcpy(dest + destLen, src, copyLen);
    dest[destLen + copyLen] = '\0';

    return destLen + srcLen;
}

//  strchr ret index
int32_t strchrInd(const char *s, int c){
	int32_t i = 0;
    while (s[i]){
        if (s[i] == (char)c)
            return i;
		i++;
	}
    return -1;
}

//  strchr
char *strchr(const char *s, int c){
    while (*s++)
        if (*s == (char)c)
            break;
    return (char *)s;
}
//  strrchr
char *strrchr(const char *s, int c){
    size_t sLen = strlen(s);
    if ((char) c == '\0')
		return (char *)(s + sLen);
    while (sLen > 0)
    {
        if (s[sLen - 1] == (char)c)
            return (char *)(s + sLen);
        sLen--;
    }
    return NULL;
}
//  strncmp
int strncmp(const char *s1, const char *s2, size_t n){
    while (--n && *s1 && *s2 && (*s1 == *s2))
        s1++, s2++;
    return *s1 - *s2;
}
//  strcmp
int strcmp(const char *s1, const char *s2){
    while (*s1 && *s2 && (*s1 == *s2))
        s1++, s2++;
    return *s1 - *s2;
}
//  strnstr
char *strnstr(const char *big, const char *little, size_t len){
    if (!*little)
        return (char *)big;
    size_t littleLen = strlen(little);
    size_t i = 0;
    while (big[i] && i < len)
    {
        if (!strncmp(big + i, little, littleLen))
            return (char *)big + i;
        i++;
    }
    return NULL;
}