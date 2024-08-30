#pragma once

# define uint16_t   unsigned short
# define uint8_t    unsigned char
# define uint32_t   unsigned int
# define int16_t    short
# define int8_t     char
# define int32_t    int

# define physAddr  uint32_t 

# define size_t   unsigned int
# define bool     uint8_t
# define true     1
# define false    0
# define NULL     0

uint8_t		isAlpha(uint8_t c);
uint8_t		isDigit(uint8_t c);
uint8_t		isPrintable(uint8_t c);
uint8_t		isLowerCase(uint8_t c);
uint8_t		isUpperCase(uint8_t c);
uint32_t	getNbrSize(uint32_t nbr, uint8_t baseLen);

# define HEX_BASE "0123456789ABCDEF"
# define DEC_BASE "0123456789"
# define BIN_BASE "01"
