#pragma once

# define uint16_t unsigned short
# define uint8_t  unsigned char
# define uint32_t unsigned int
# define int16_t  short
# define int8_t   char
# define int32_t  int

# define size_t   unsigned int
# define bool     uint8_t
# define true     1
# define false    0
# define NULL     0


# define L8(VAL) (VAL & 0xFF)
# define L16(VAL) (VAL & 0xFFFF)
# define H8(VAL) ((VAL >> 8) & 0xFF)
# define H16(VAL) ((VAL >> 16) & 0xFFFF)

uint8_t	isAlpha(uint8_t c);
uint8_t	isDigit(uint8_t c);
uint8_t	isPrintable(uint8_t c);