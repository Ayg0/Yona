#include "klibc/types.h"
#include "klibc/strings.h"
#include "klibc/print.h"
#include "drivers/time.h"
#include "klibc/converts.h"
#include "yonaShell/yonaShell.h"

extern _yonaData yona;

void    echo(char *args){
    if (!strcmp(args, "--help")) {
        PRINT_K("Usage: echo <string>\n\r", NULL);
        return;
    }

    while (*args) {
        PRINT_K("%c", *args);
        msSleep(50);
        args++;
    }
    PRINT_K("\n\r", NULL);
}

void    peek(char *args){
    uint32_t    addr;

    if (!strcmp(args, "--help")){
        PRINT_K("Usage: peek <address [HEX]>\n\r", NULL);
        return;
    }

    addr = aHextoiS(args, NULL);
    PRINT_K("%08p: HEX: [%x], DEC: [%d], ASCII [%c]\n\r", addr, *(char *)addr, *(char *)addr, *(char *)addr);
}

void    poke(char *args){
    uint32_t    addr;
    uint8_t     value;

    if (!strcmp(args, "--help")){
        PRINT_K("Usage: poke <address [HEX]> <value [DEC]>\n\r", NULL);
        return;
    }
    addr = aHextoiS(strtok(args, " "), NULL);
    value = atoiS(strtok(NULL, " "), NULL);

    *(uint8_t *)addr = value;
    PRINT_K("Wrote %d to %08p\n\r", value, addr);
}

void reboot(char *args) {
    (void)args;

    if (!strcmp(args, "--help")){
        PRINT_K("man niytak ? just do it.\n\r", NULL);
        return;
    }
    uint8_t ready;
	do{
        ready = pByteIn(0x64);
	} while (ready & 0x02);	// wait until keyboard is ready to accept a command.
    pByteOut(0x64, 0xFE);	// send 0xFE command that causes the system to reboot.
    asm volatile ("hlt");
}

void	dumpMemory(uint32_t start, uint32_t size){
    
    uint8_t *addr = (uint8_t *)start;
    uint8_t *endAddr = addr + size;
    char *color;

    while (addr < endAddr)
    {
        // print address
        PRINT_K("%08p ", addr);
        PRINT_SC("%08p ", addr);
        // print hex values
        for (uint8_t i = 0; i < 16; i++) {
            color = (addr[i] == 0) ? COLOR_WHITE : isPrintable(addr[i]) ? COLOR_GREEN : COLOR_RED;
            PRINT_K("%s%02x\033[0m", color, addr[i]);
            PRINT_SC("%s%02x\033[0m", color, addr[i]);
            if (i % 2)
                PRINT_K(" ", NULL), PRINT_SC(" ", NULL);
        }
        PRINT_K("  ", NULL), PRINT_SC("  ", NULL);
        // print ascii values
        for (uint8_t i = 0; i < 16; i++) {
            color = (addr[i] == 0) ? COLOR_WHITE : isPrintable(addr[i]) ? COLOR_GREEN : COLOR_RED;
            PRINT_K("%s%c\033[0m", color, isPrintable(addr[i]) ? addr[i] : '.');
            PRINT_SC("%s%c\033[0m", color, isPrintable(addr[i]) ? addr[i] : '.');
        }
        addr += 16;
        PRINT_K("\n\r", NULL), PRINT_SC("\n\r", NULL);
    }
}

void    dumpCmd(char *args){
    size_t    addr;
    size_t     size;

    if (!strcmp(args, "--help")){
        PRINT_K("Usage: dump <address [HEX]> <size [DEC]>\n\r", NULL);
        return;
    }

    addr = aHextoiS(strtok(args, " "), NULL);
    size = atoiS(strtok(NULL, " "), NULL);

    dumpMemory(addr, size);
}

void    hltCmd(char *args){

    if (!strcmp(args, "--help")){
        PRINT_K("Usage: Zam takram ala mitasyion. jmaaaaaaad\n\r", NULL);
        return;
    }

    S_ERR("Halting the system\n", NULL);
    PRINT_K("Halting the system\n\r", NULL);
    setYonaStatus(YONA_STATUS_UNKNOWN);
    // disable interrupts
    asm volatile("cli");
    // halt the system
    asm volatile("hlt");
}

void    clearTty(char *args){
    (void)args;
    if (!strcmp(args, "--help")){
        PRINT_K("Usage: clear the current TTY\n\r", NULL);
        return;
    }
    clearScreenBuffer();
}

void    printStack(char *args){
    (void)args;
    size_t esp;
    
	volatile char s[] = "Hii I'm in the printStack Function";
    (void)s;

    if (!strcmp(args, "--help")) {
        PRINT_K("Usage: print the current stack\n\r", NULL);
        return;
    }
    __asm__ __volatile__("mov %%esp, %0" : "=r" (esp));

    PRINT_K("Stack:\n\r", NULL);
    S_DEBUG("EBP: %8p\n", yona.mainEBP);
    S_DEBUG("ESP: %8p\n", esp);
    dumpMemory(esp, yona.mainEBP - esp);
}