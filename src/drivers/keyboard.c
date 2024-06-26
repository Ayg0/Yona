#include "keyboard.h"
#include "serialio.h"
#include "tty.h"
#include "mem.h"
#include "shell.h"

_kbdFlags flags;
extern _kbdLayout kbd_US_QWERTY;
extern _tty	tty;
volatile _kbdBuffer	*currentBuffer = NULL;


void	backSpace(){
	_pos currentCursor = tty.currentSession->cursor;
	if (currentBuffer->position == 0)
		return ;
	currentBuffer->position--;
	currentBuffer->buffer[currentBuffer->position] = 0;
	setTtyCursor(currentCursor.x - 1, currentCursor.y);
	ttyAddCharPos(' ', currentCursor.x - 1, currentCursor.y);
}

void handleCtrl(uint8_t scanCode, uint8_t letter){
	(void) scanCode;
	if (letter >= '1' && letter <= '6')
		switchSession(letter - 49), initKeboard(letter - 49);
	else if ((letter == 'R' || letter == 'r') && IS_SET(flags.modifiers, SHIFT_MOD))
		reboot();
}

void	getFunctionLetters(uint8_t scancode, uint32_t letter){
	switch (scancode)
	{
	case 0x0E:
		backSpace();
		break;
	case 0x2A:
		SET_BIT(flags.modifiers, SHIFT_MOD);
		break;
	case 0x36:
		SET_BIT(flags.modifiers, SHIFT_MOD);
		break;
	case 0x3A:
		if (IS_SET(flags.modifiers, CAPS_MOD))
			RESET_BIT(flags.modifiers, CAPS_MOD);
		else
			SET_BIT(flags.modifiers, CAPS_MOD);
		break;
	case 0xAA:
		RESET_BIT(flags.modifiers, SHIFT_MOD);
		break;
	case 0xB6:
		RESET_BIT(flags.modifiers, SHIFT_MOD);
		break;
	case 0x1D:
		SET_BIT(flags.modifiers, CTRL_MOD);
		break;
	case 0x9D:
		RESET_BIT(flags.modifiers, CTRL_MOD);
		break;
	default: break;
	}
	handleCtrl(scancode, letter);
}

uint8_t	getLetter(uint8_t	scanCode){
	uint8_t letter = 0;
	letter = kbd_US_QWERTY.views[IS_SET(flags.modifiers, SHIFT_MOD)][scanCode];
	if (isAlpha(letter) && IS_SET(flags.modifiers, CAPS_MOD))
		letter -= 32 * ((letter >= 'a') + -1 * (letter < 'a'));
	return letter;
}

uint8_t	appendLetter(uint8_t letter){

	if (currentBuffer->position == 255)
		return 0;
	if (letter == '\n')
			appendLetter('\r');
	ttyAddChar(letter);
	currentBuffer->buffer[currentBuffer->position] = letter;
	currentBuffer->position += 1;

	return letter;
}

void	keyboardHandler(registers Rs){
	(void) Rs;
	uint8_t	letter = 0;
    uint8_t scanCode =  PbyteIn(0x60); // where PIC leave the scancode

	if (scanCode & 0x80)
		goto HANDLE_SPECIAL_KEYS;

	letter = getLetter(scanCode);
	if (IS_SET(flags.modifiers, CTRL_MOD))
		goto HANDLE_SPECIAL_KEYS;
	else if (letter)
		appendLetter(letter);
	else
	HANDLE_SPECIAL_KEYS:
		getFunctionLetters(scanCode, letter);
}

void	initKeboard(uint8_t sessionIndex){
	currentBuffer = &tty.Sessions[sessionIndex].kbdBuffer;
}

void	clearBuffer(){
	bzero((void *)currentBuffer, sizeof(_kbdBuffer));
}

char	*input(char *declare){
	if (declare){
		uint8_t clr = tty.color;
		changeTtyColor(VGA_YELLOW, -1);
		printfTty(declare);
		tty.color = clr;
	}
	while (currentBuffer->buffer[currentBuffer->position - 1] != '\n')
		    asm volatile("" : : : "memory"); // to provent compiler from optimizing and assuming the mem.
	currentBuffer->buffer[currentBuffer->position - 2] = 0;
	return (char *)currentBuffer->buffer;
}