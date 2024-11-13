#pragma once

# include "klibc/types.h"
# include "arch/ports.h"
# include "arch/i386/idts.h"

# define KBD_DATA 0x60
# define KBD_STAT 0x64

// keyboard flags
# define SHIFT_MODIFIER     0
# define CTRL_MODIFIER      1
# define CAPS_MODIFIER      2
# define NUMLOCK_MODIFIER   3
# define NEWLINE            4


typedef struct keyboardViews
{
	uint8_t	*keys;
	uint8_t	*shiftedKeys;
} _keyboardViews;

typedef union kbdLayout
{
	struct keyboardViews kbdV;
	uint8_t *views[2];
} _kbdLayout;


typedef struct kbdBuffer
{
    uint8_t	buffer[256];
    uint32_t index;
} _kbdBuffer;

typedef struct keyboardData
{
    _kbdLayout	layout;
    _kbdBuffer  buffer;
    uint8_t     kbdFlags;
} _keyboardData;

void initKeyboard();

char	*prompt(char *declare, char *buffer);
void    clearKeyboardBuffer();