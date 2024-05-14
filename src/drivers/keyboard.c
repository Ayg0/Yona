#include "keyboard.h"
#include "serialPorts.h"
_keyboardFlags flags;

// this array map was provided thanks to: https://stackoverflow.com/questions/61124564/convert-scancodes-to-ascii
char kbd_US[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
	'\b',	/* <-- back space */   
  '\t',		/* <-- Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

char Shifted_kbd_US[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
	'\b',	/* <-- back space */   
  '\t',		/* <-- Tab */
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     
    0, /* <-- control key */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '`',  0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

uint8_t	getFunctionLetters(uint8_t scancode){
	switch (scancode)
	{
	case 0x1C:
		flags.nl = 1;
		// new_line();
		return 0;
	case 0x0D: return '=';
	case 0x0E:
		// return back_space();
		return 0;
	case 0x2A:
		SET_BIT(flags.modifiers, SHIFT_MOD);
		return 0;
	case 0x36:
		SET_BIT(flags.modifiers, SHIFT_MOD);
		return 0;
	case 0x3A:
		if (IS_SET(flags.modifiers, CAPS_MOD))
			SET_BIT(flags.modifiers, CAPS_MOD);
		else
			RESET_BIT(flags.modifiers, CAPS_MOD);
		return 0;
	case 0xAA:
		RESET_BIT(flags.modifiers, SHIFT_MOD);
		return 0;
	case 0xB6:
		RESET_BIT(flags.modifiers, SHIFT_MOD);
		return 0;
	case 0x1D:
		SET_BIT(flags.modifiers, CTRL_MOD);
		return 0;
	case 0x9D:
		RESET_BIT(flags.modifiers, CTRL_MOD);
		return 0;
	default: return 0;
	}
}

#include "tty.h"
void	keyboardHandler(registers Rs){
    uint8_t scanCode =  PbyteIn(0x60); // where PIC leave the scancode
	uint8_t	letter = 0;

	if (scanCode & 0x80)
		goto HANDLE_SPECIAL_KEYS;
	if (IS_SET(flags.modifiers, SHIFT_MOD))
		letter = Shifted_kbd_US[scanCode];
	else
		letter = kbd_US[scanCode];
	(void) Rs;
	if (letter >= 'a' && letter <= 'z' && CAP_OPT(flags.modifiers))
		letter -= 32;
	if (IS_SET(flags.modifiers, CTRL_MOD) && letter >= '1' && letter <= '6')
		switchSession(letter - 49);
	else if (letter){
		if (letter == '\n')
			ttyAddChar('\r');
		ttyAddChar(letter);
	}
	else
	HANDLE_SPECIAL_KEYS:
		getFunctionLetters(scanCode);
}