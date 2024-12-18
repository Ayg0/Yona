#pragma once

# define uint16_t   unsigned short
# define uint8_t    unsigned char
# define uint32_t   unsigned int
# define int16_t    short
# define int8_t     char
# define int32_t    int
# define size_t   uint32_t

# define bool  uint8_t
# define true     1
# define false    0
# define NULL     0

uint8_t		isAlpha(uint8_t c);
uint8_t		isDigit(uint8_t c);
uint8_t		isPrintable(uint8_t c);
uint8_t		isLowerCase(uint8_t c);
uint8_t		isUpperCase(uint8_t c);
uint8_t     toLowerCase(uint8_t c);
uint8_t     toUpperCase(uint8_t c);
uint32_t	getNbrSize(uint32_t nbr, uint8_t baseLen);

// base strings
# define HEX_BASE "0123456789ABCDEF"
# define DEC_BASE "0123456789"
# define BIN_BASE "01"

// bit manipulation
# define BIT_SET(FLAG, BIT) FLAG = FLAG | (1 << BIT)
# define BIT_RESET(FLAG, BIT) FLAG = FLAG & ~(1 << BIT)
# define BIT_IS_SET(FLAG, BIT) (FLAG & (1 << BIT))

// byte manipulation
# define L8(VAL) (VAL & 0xFF)
# define L16(VAL) (VAL & 0xFFFF)
# define H8(VAL) ((VAL >> 8) & 0xFF)
# define H16(VAL) ((VAL >> 16) & 0xFFFF)
