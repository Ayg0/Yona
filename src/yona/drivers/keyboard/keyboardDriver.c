#include "klibc/print.h"
#include "klibc/memory.h"
#include "klibc/strings.h"
#include "drivers/keyboard.h"
#include "drivers/vga/textMode/vgaTextMode.h"

_keyboardData kbdData;
extern _kbdLayout kbd_US_QWERTY;
extern _ttySession tty;
uint8_t currentIndexInHistory = 0;

void    popFirstHistoryCommand(){
    char *currentBuff, *nextBuff;
    for (uint8_t i = 0; i < KBD_HISTORY_SIZE - 1; i++){
        currentBuff = (char *)kbdData.historyBuffer[i].buffer;
        nextBuff = (char *)kbdData.historyBuffer[i + 1].buffer;
        strlcpy(currentBuff, nextBuff, KBD_BUFFER_SIZE);
        kbdData.historyBuffer[i].index = kbdData.historyBuffer[i + 1].index;
    }
    kbdData.historyBuffer[KBD_HISTORY_SIZE - 1].index = 0;
    bzero(kbdData.historyBuffer[KBD_HISTORY_SIZE - 1].buffer, KBD_BUFFER_SIZE);
}

void addToHistory(){
    uint8_t index = kbdData.historyIndex;
    if (index >= 128){
        popFirstHistoryCommand();
        index = 127;
    }
    if (*kbdData.buffer.buffer == 0)
        return ;
    char *buffToSaveHisto = (char *)kbdData.historyBuffer[index].buffer;
    strlcpy(buffToSaveHisto, (char *)kbdData.buffer.buffer, KBD_BUFFER_SIZE);
    kbdData.historyBuffer[index].index = kbdData.buffer.index;
    kbdData.historyIndex = index + 1;
    currentIndexInHistory = index + 1;
    S_DEBUG("Added to history: %s\n", kbdData.buffer.buffer);
}

void clearUserInput(){
    uint8_t currentX = tty.cursor.x;
    uint8_t currentY = tty.cursor.y;
    uint8_t buffferSize = strlen((char *)kbdData.buffer.buffer);

    for (uint8_t i = currentX - buffferSize; i < currentX; i++)
        kPutPosC(' ', i, currentY);
    clearKeyboardBuffer();
    moveCursor(currentX - buffferSize, currentY);
}

void spawnCommand(int16_t index){
    if (index < 0 || index >= kbdData.historyIndex)
        return ;
    clearUserInput();

    // copy the history command to the buffer
    strlcpy((char *)kbdData.buffer.buffer, (char *)kbdData.historyBuffer[index].buffer, KBD_BUFFER_SIZE);
    // move the cursor back to the start of the prompt:
    moveCursor(tty.cursor.x + kbdData.buffer.index, tty.cursor.y);
    // print the command and set the index
    kbdData.buffer.index = kbdData.historyBuffer[index].index;
    PRINT_K("%s", kbdData.buffer.buffer);
    // set the current index in history
    currentIndexInHistory = index;
}

uint8_t getLetterFromScanCode(uint8_t scanCode){
    uint8_t letter = 0;
    bool    selectedLayout;

    // select the keyboard layout based on shift key
    selectedLayout = BIT_IS_SET(kbdData.kbdFlags, SHIFT_MODIFIER) ? 1 : 0;

    // get letter from the keyboard layout
    letter = kbdData.layout.views[selectedLayout][scanCode];

    // if caps lock is on, invert letter case
	if (isAlpha(letter) && BIT_IS_SET(kbdData.kbdFlags, CAPS_MODIFIER))
		letter -= 32 * ((letter >= 'a') + -1 * (letter < 'a'));
	return letter;
}

uint8_t getScanCodeFromLetter(uint8_t letter){
    bool    selectedLayout;
    uint8_t scanCode = 0;

    // select the keyboard layout based on shift key
    selectedLayout = BIT_IS_SET(kbdData.kbdFlags, SHIFT_MODIFIER) ? 1 : 0;

    // get letter from the keyboard layout
    for (uint8_t i = 0; i < 128; i++){
        if (kbdData.layout.views[selectedLayout][i] == letter){
            scanCode = i;
            break;
        }
    }
    return scanCode;
}

void    handleKey(uint8_t letter){
    if (letter == '\n'){
        BIT_SET(kbdData.kbdFlags, NEWLINE);
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
    case 0x48:
        spawnCommand(currentIndexInHistory - 1);
        break;
    case 0x50:
        spawnCommand(currentIndexInHistory + 1);
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
        return kbdData.keyReleaseHandler(scanCode);
    letter = getLetterFromScanCode(scanCode);

    // handle ctrl+key
    if (BIT_IS_SET(kbdData.kbdFlags, CTRL_MODIFIER))
        return handleCtrl(scanCode);
    else if (letter) // handle letter
        kbdData.keyPressHandler(letter);
    else // handle special keys (shift, caps lock, backspace, etc.)
        handleSpecialKeys(scanCode);
}

// null restores the default handler
void    setKeyPressHandler(onKeyPressHanlder handler){
    if (handler)
        kbdData.keyPressHandler = handler;
    else
        kbdData.keyPressHandler = handleKey;
}

// null restores the default handler
void    setKeyReleaseHandler(onKeyReleaseHandler handler){
    if (handler)
        kbdData.keyReleaseHandler = handler;
    else
        kbdData.keyReleaseHandler = handleKeyRelease;
}

void initKeyboard(){
    kbdData.layout = kbd_US_QWERTY;
    kbdData.kbdFlags = 0;
    kbdData.buffer.index = 0;
    kbdData.keyPressHandler = handleKey;
    kbdData.keyReleaseHandler = handleKeyRelease;
    setIRQHandler(1, keyboardHandler);
}

void inturruptPrompting(){
    BIT_SET(kbdData.kbdFlags, NEWLINE);
}

void	clearKeyboardBuffer(){
    bzero(kbdData.buffer.buffer, KBD_BUFFER_SIZE);
    kbdData.buffer.index = 0;
}

char	*prompt(char *declare, char *buffer){
	if (declare)
        PRINT_K(COLOR_LIGHT_BLUE"%s>"COLOR_DEFAULT" ", declare);
    while (!BIT_IS_SET(kbdData.kbdFlags, NEWLINE))
        asm volatile("" : : : "memory");

    BIT_RESET(kbdData.kbdFlags, NEWLINE);
    strlcpy(buffer, (char *)kbdData.buffer.buffer, KBD_BUFFER_SIZE);
    addToHistory();
    clearKeyboardBuffer();
    PRINT_K("\n\r", NULL);
    return buffer;
}