#include "keyboard.h"
#include "serialPorts.h"
_kbdFlags flags;
extern _kbdLayout kbd_US_QWERTY;

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
			RESET_BIT(flags.modifiers, CAPS_MOD);
		else
			SET_BIT(flags.modifiers, CAPS_MOD);
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
	(void) Rs;
	uint8_t	letter = 0;
    uint8_t scanCode =  PbyteIn(0x60); // where PIC leave the scancode

	if (scanCode & 0x80)
		goto HANDLE_SPECIAL_KEYS;

	letter = kbd_US_QWERTY.views[IS_SET(flags.modifiers, SHIFT_MOD)][scanCode];
	if (isAlpha(letter) && IS_SET(flags.modifiers, CAPS_MOD))
		letter -= 32 * ((letter >= 'a') + -1 * (letter < 'a'));
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

void	initKeyboard(){

}

void	switchLayout(){	// didn't code it yet

}