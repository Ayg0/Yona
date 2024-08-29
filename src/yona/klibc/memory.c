#include "klibc/memory.h"

void *memcpy(void *dest, const void *src, size_t n){
    size_t i = 0;

    char *destC = dest;
    const char *srcC = src;
    while (i < n)
    {
        destC[i] = srcC[i];
        i++;
    }
    return dest;    
}
//  memset
void    *memset(void *s, int c, size_t n){
    size_t i = 0;
    char *sC = s;

    while (i < n)
    {
        sC[i] = (char)c;
        i++;
    }
    return s;
}
//  bzero
void    bzero(void *s, size_t n){
    memset(s, 0, n);
}
//  memmove
void *memmove(void *dest, const void *src, size_t n){
    size_t i = 0;
    char *destC = dest;
    const char *srcC = src;

    if (destC < srcC)
    {
        while (i < n)
        {
            destC[i] = srcC[i];
            i++;
        }
    }
    else
    {
        while (n > 0)
        {
            n--;
            destC[n] = srcC[n];
        }
    }
    return dest;
}    
//  memchr
void *memchr(const void *s, int c, size_t n){
    size_t i = 0;
    const char *sC = s;

    while (i < n)
    {
        if (sC[i] == (char)c)
            return (void *)sC + i;
        i++;
    }
    return NULL;
}
//  memcmp
int memcmp(const void *s1, const void *s2, size_t n){
    size_t i = 0;
    const char *s1C = s1;
    const char *s2C = s2;

    while (i < n)
    {
        if (s1C[i] != s2C[i])
            return s1C[i] - s2C[i];
        i++;
    }
    return 0;
}