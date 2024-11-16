src
├── arch
│   └── i386
│       ├── boot.s
│       ├── cpu
│       │   ├── gdt.c
│       │   ├── gdtLoader.s
│       │   ├── idt.c
│       │   ├── irqs.c
│       │   └── ISRs.s
│       ├── grub.cfg
│       ├── linker.ld
│       └── serialPorts
│           ├── serialCOM1.c
│           └── serialIO.c
├── include
│   ├── arch
│   │   ├── i386
│   │   │   └── idts.h
│   │   ├── ports.h
│   │   └── serial.h
│   ├── drivers
│   │   ├── keyboard.h
│   │   ├── time.h
│   │   └── vga
│   │       └── textMode
│   │           └── vgaTextMode.h
│   ├── klibc
│   │   ├── converts.h
│   │   ├── memory.h
│   │   ├── print.h
│   │   ├── strings.h
│   │   ├── types.h
│   │   └── vargs.h
│   ├── yonaData.h
│   └── yonaShell
│       └── yonaShell.h
└── yona
    ├── drivers
    │   ├── keyboard
    │   │   ├── ctrlkeyMaps.c
    │   │   ├── keyboardDriver.c
    │   │   └── layouts
    │   │       └── kbd_US.c
    │   ├── timer
    │   │   └── timerDriver.c
    │   └── vga
    │       └── textMode
    │           ├── cursor.c
    │           └── kputs.c
    ├── kernel.c
    ├── klibc
    │   ├── converts.c
    │   ├── memory.c
    │   ├── print.c
    │   ├── strings.c
    │   └── types.c
    └── yonaShell
        ├── commands.c
        └── yonaShell.c

22 directories, 38 files
