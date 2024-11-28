YonaOs is a 32-bit hobby operating system made in C. This is my 4th attempt at building Yona, with fewer mistakes and new design decisions.

## Features
- ANSI-like escape sequences to handle coloring.
- using serial communication for debugging.
- printf and sprintf clones for both the VGA and serial communication.
- as simple as possible to play test all you need is to use 'make'.
- a minimalistic shell with various commands and games.

## Text Mode Colors
To change color, just print `"\033[<color-ID>m"`. Here are the available colors:

| color-ID | color            |
|----------|------------------|
| 30       | VGA_BLACK        |
| 34       | VGA_BLUE         |
| 32       | VGA_GREEN        |
| 36       | VGA_CYAN         |
| 31       | VGA_RED          |
| 35       | VGA_MAGENTA      |
| 33       | VGA_BROWN        |
| 37       | VGA_LIGHT_GREY   |
| 90       | VGA_DARK_GREY    |
| 94       | VGA_LIGHT_BLUE   |
| 92       | VGA_LIGHT_GREEN  |
| 96       | VGA_LIGHT_CYAN   |
| 91       | VGA_LIGHT_RED    |
| 95       | VGA_LIGHT_MAGENTA|
| 93       | VGA_YELLOW       |
| 97       | VGA_WHITE        |
| 39       | VGA_DEFAULT      |

## General Resources
I have created a resources folder for you.

### Building the Cross-Compiler
- [Building GCC](https://wiki.osdev.org/Building_GCC)
- [GCC Cross Compiler Guide](https://ayg0.github.io/gettoknow.github.io/posts/gcccrosscompiler/)

## Play-Testing the OS
The only prerequisite you need is Docker and Docker Compose. I have made an image available on Docker Hub: `0rayne/i386.env:v1.0`. that has the cross compiler and the necessary tools to build the OS.

### Commands
To make the ISO, run:
```sh
make
```

To clean the binaries, run:
```sh
make fclean
```

To run the OS in QEMU, run:
```sh
make run
```
NOTE: of course, you need to have QEMU installed.
