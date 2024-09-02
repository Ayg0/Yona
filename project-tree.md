src
├── arch
│   └── i386
│       ├── boot.s
│       ├── grub.cfg
│       ├── linker.ld
│       └── serialPorts
│           ├── serialCOM1.c
│           └── serialIOs.c
├── include
│   ├── arch
│   │   └── serial.h
│   ├── drivers
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

15 directories, 21 files
