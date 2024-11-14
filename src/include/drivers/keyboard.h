#pragma once

# include "klibc/types.h"
# include "arch/ports.h"
# include "arch/i386/idts.h"

# define KBD_DATA 0x60
# define KBD_STAT 0x64

# define KBD_BUFFER_SIZE 80
# define KBD_HISTORY_SIZE 128

typedef void (*ctrlKeyFunc)(void);

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
    uint8_t     buffer[80];
    uint32_t    index;
} _kbdBuffer;

typedef struct keyboardData
{
    _kbdLayout	layout;
    _kbdBuffer  buffer;
    _kbdBuffer  historyBuffer[128];
    uint8_t     historyIndex;
    uint8_t     kbdFlags;
} _keyboardData;



void initKeyboard();

// keyboard buffer related:
char    *prompt(char *declare, char *buffer);
void    clearKeyboardBuffer();
// ctrl key related:
void handleCtrl(uint8_t scanCode);
void setCtrlFunction(uint8_t scanCode, ctrlKeyFunc func);
// keyboard handler related:
void inturruptPrompting();
// scanCode related:
uint8_t getLetterFromScanCode(uint8_t scanCode);
uint8_t getScanCodeFromLetter(uint8_t letter);
