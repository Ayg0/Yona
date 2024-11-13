#include "klibc/print.h"
#include "klibc/strings.h"
#include "klibc/memory.h"
#include "drivers/keyboard.h"

_keyboardData kbdData;
extern _kbdLayout kbd_US_QWERTY;


uint8_t    getLetterFromScanCode(uint8_t scanCode){
    uint8_t letter = 0;
    bool    selectedLayout;

    // select the keyboard layout based on shift key
    selectedLayout = BIT_IS_SET(kbdData.kbdFlags, SHIFT_MODIFIER) ? 1 : 0;

    // get letter from the keyboard layout
    letter = kbdData.layout.views[selectedLayout][scanCode];

    S_DEBUG("layout %d, letter %c\n", selectedLayout, letter);
    // if caps lock is on, invert letter case
	if (isAlpha(letter) && BIT_IS_SET(kbdData.kbdFlags, CAPS_MODIFIER))
		letter -= 32 * ((letter >= 'a') + -1 * (letter < 'a'));
	return letter;
}

void    handleKey(uint8_t letter){
    if (letter == '\n'){
        BIT_SET(kbdData.kbdFlags, NEWLINE);
        S_INFO("NEWLINE FLAG %d\n", BIT_IS_SET(kbdData.kbdFlags, NEWLINE));
        return ;
    }

    // check for buffer overflow
    else if (kbdData.buffer.index >= 256)
        S_ERR("Buffer is full\n", NULL);
    else {
        kbdData.buffer.buffer[kbdData.buffer.index] = letter;
        kbdData.buffer.index++;
    }
    PRINT_K("%c", letter);
}

void handleKeyRelease(uint8_t scanCode){
    switch (scanCode) {
	case 0xAA:
        BIT_RESET(kbdData.kbdFlags, SHIFT_MODIFIER);
		break;
	case 0xB6:
        BIT_RESET(kbdData.kbdFlags, SHIFT_MODIFIER);
		break;
	case 0x9D:
        BIT_RESET(kbdData.kbdFlags, CTRL_MODIFIER);
	default: break;
	}
}

void handleBackSpace(){
    if (kbdData.buffer.index > 0) {
        kbdData.buffer.index--;
        kbdData.buffer.buffer[kbdData.buffer.index] = 0;
        PRINT_K("\b", NULL);
    }
}

void handleCtrl(uint8_t scanCode, uint8_t letter){
    (void)scanCode;
    if (letter >= 'a' && letter <= 'z')
        letter -= 32;
    handleKey('^');
    handleKey(letter);
}

void handleSpecialKeys(uint8_t scanCode){

    switch (scanCode) {
	case 0x0E:
		handleBackSpace();
		break;
	case 0x2A:
        BIT_SET(kbdData.kbdFlags, SHIFT_MODIFIER);
		break;
	case 0x36:
        BIT_SET(kbdData.kbdFlags, SHIFT_MODIFIER);
		break;
	case 0x3A:
		if (BIT_IS_SET(kbdData.kbdFlags, CAPS_MODIFIER))
			BIT_RESET(kbdData.kbdFlags, CAPS_MODIFIER);
		else
			BIT_SET(kbdData.kbdFlags, CAPS_MODIFIER);
		break;
	case 0x1D:
	    BIT_SET(kbdData.kbdFlags, CTRL_MODIFIER);
		break;
	default: break;
	}
}

void    keyboardHandler(_registers r){

    (void)r;
    uint8_t scanCode = pByteIn(KBD_DATA);
    uint8_t letter = 0;

    // check for key release

    if (scanCode & 0x80)
        return handleKeyRelease(scanCode);
    letter = getLetterFromScanCode(scanCode);

    // handle ctrl+key
    if (BIT_IS_SET(kbdData.kbdFlags, CTRL_MODIFIER))
        return handleCtrl(scanCode, letter);
    else if (letter) // handle letter
        handleKey(letter);
    else // handle special keys (shift, caps lock, backspace, etc.)
        handleSpecialKeys(scanCode);
}

void initKeyboard(){
    kbdData.layout = kbd_US_QWERTY;
    kbdData.kbdFlags = 0;
    kbdData.buffer.index = 0;
    setIRQHandler(1, keyboardHandler);
}

void	clearKeyboardBuffer(){
    bzero(kbdData.buffer.buffer, 256);
    kbdData.buffer.index = 0;
}

char	*prompt(char *declare, char *buffer){
	if (declare)
        PRINT_K("\033[34m%s>\033[39m ", declare);
    while (!BIT_IS_SET(kbdData.kbdFlags, NEWLINE))
        asm volatile("" : : : "memory");

    BIT_RESET(kbdData.kbdFlags, NEWLINE);
    strlcpy(buffer, (char *)kbdData.buffer.buffer, 256);
    clearKeyboardBuffer();
    return buffer;
}