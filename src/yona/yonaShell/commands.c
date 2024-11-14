#include "klibc/types.h"
#include "klibc/strings.h"
#include "klibc/print.h"
#include "klibc/converts.h"
#include "yonaShell/yonaShell.h"


void    echo(char *args){
    PRINT_K("%s\n\r", args);
}

void    peek(char *args){
    uint32_t    addr;

    addr = aHextoiS(args, NULL);
    PRINT_K("%p: HEX: [%x], DEC: [%d], ASCII [%c]\n\r", addr, *(char *)addr, *(char *)addr, *(char *)addr);
}

void    poke(char *args){
    uint32_t    addr;
    uint8_t     value;

    addr = aHextoiS(strtok(args, " "), NULL);
    value = atoiS(strtok(NULL, " "), NULL);

    *(uint8_t *)addr = value;
    PRINT_K("Wrote %x to %p\n\r", value, addr);
}

void reboot(char *args) {
    (void)args;
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
        PRINT_K("%8p ", addr);
        // print hex values
        for (uint8_t i = 0; i < 16; i++) {
            color = (addr[i] == 0) ? COLOR_WHITE : isPrintable(addr[i]) ? COLOR_GREEN : COLOR_RED;
            PRINT_K("%s%02x\033[0m", color, addr[i]);
            if (i % 2)
                PRINT_K(" ", NULL);
        }
        PRINT_K("  ", NULL);
        // print ascii values
        for (uint8_t i = 0; i < 16; i++) {
            color = (addr[i] == 0) ? COLOR_WHITE : isPrintable(addr[i]) ? COLOR_GREEN : COLOR_RED;
            PRINT_K("%s%c\033[0m", color, isPrintable(addr[i]) ? addr[i] : '.');
        }
        addr += 16;
        PRINT_K("\n\r", NULL);
    }
}

void    dumpCmd(char *args){
    size_t    addr;
    size_t     size;

    addr = aHextoiS(strtok(args, " "), NULL);
    size = atoiS(strtok(NULL, " "), NULL);

    dumpMemory(addr, size);
}

void    hltCmd(char *args){
    (void)args;
    S_ERR("Halting the system\n", NULL);

    setYonaStatus(YONA_STATUS_UNKNOWN);
    // disable interrupts
    asm volatile("cli");
    // halt the system
    asm volatile("hlt");
}