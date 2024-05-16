#pragma once

#include "types.h"
#include "ports.h"
#include "CPU/DiscriptorTables.h"

# define KBD_DATA 0x60
# define KBD_STAT 0x64

enum MODIFIER_KEYS {
	SHIFT_MOD,
	CTRL_MOD,
	CAPS_MOD,
	NUMLOCK_MOD,
};
struct keyboardViews
{
	uint8_t	*keys;
	uint8_t	*shiftedKeys;
};

typedef union kbdLayout
{
	struct keyboardViews kbdV;
	uint8_t *views[2];
} _kbdLayout;


typedef struct kbdFlags
{
	uint8_t	nl;
	uint8_t	alt;
	uint8_t	modifiers;
} _kbdFlags;

typedef struct kbdBuffer
{
	int8_t		buffer[256];
	uint8_t		position;
} _kbdBuffer;


# define SET_BIT(FLAG, BIT) (FLAG = FLAG | 1 << BIT)
# define RESET_BIT(FLAG, BIT) (FLAG = FLAG & ~(1 << BIT))
# define IS_SET(FLAG, BIT) (FLAG & (1 << BIT))

void	keyboardHandler(registers Rs);
char	*input(char *declare);
void	clearBuffer();
void	initKeboard(uint8_t sessionIndex);

