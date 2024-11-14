#include "drivers/keyboard.h"
#include "klibc/print.h"

ctrlKeyFunc ctrlKeys[128] = {0};

void setCtrlFunction(uint8_t scanCode, ctrlKeyFunc func){
    ctrlKeys[scanCode] = func;
}

void handleCtrl(uint8_t scanCode){
    if (ctrlKeys[scanCode])
        ctrlKeys[scanCode]();
}
