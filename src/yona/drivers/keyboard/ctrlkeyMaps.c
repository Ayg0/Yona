#include "klibc/types.h"
#include "klibc/print.h"
#include "drivers/keyboard.h"

ctrlKeyFunc ctrlKeys[128] = {0};

void setCtrlFunction(uint8_t scanCode, ctrlKeyFunc func){
    ctrlKeys[scanCode] = func;
}

void handleCtrl(uint8_t scanCode){
    PRINT_K("^%c",  toUpperCase(getLetterFromScanCode(scanCode)));
    if (ctrlKeys[scanCode])
        ctrlKeys[scanCode]();
}
