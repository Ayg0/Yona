```
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
│   │   └── vargs.h
│   └── types.h
└── yona
    ├── drivers
    │   ├── keyboard
    │   │   ├── keyboardDriver.c
    │   │   ├── layouts
    │   │   │   └── kbd_US.c
    │   │   └── project-tree.md
    │   ├── timer
    │   │   └── timerDriver.c
    │   └── vga
    │       └── textMode
    │           ├── cursor.c
    │           └── kputs.c
    ├── kernel.c
    └── klibc
        ├── converts.c
        ├── memory.c
        ├── print.c
        ├── strings.c
        └── types.c

20 directories, 34 files
```