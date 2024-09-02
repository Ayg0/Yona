# Yona
YonaOs is a 32 bit hobby system made in C, this is my ~~3rd~~ 4th attempt at building Yona, less mistakes and new design decisions.
# Features:
  - ansi-like escape sequence to handle coloring.
# textMode Colors:
  - to change color just print "\033[\<color-ID>m", and here are the available colors:

    | color-ID | color           |
    |----------|-----------------|
    | 30       | VGA_BLACK       |
    | 34       | VGA_BLUE        |
    | 32       | VGA_GREEN       |
    | 36       | VGA_CYAN        |
    | 31       | VGA_RED         |
    | 35       | VGA_MAGENTA     |
    | 33       | VGA_BROWN       |
    | 37       | VGA_LIGHT_GREY  |
    | 90       | VGA_DARK_GREY   |
    | 94       | VGA_LIGHT_BLUE  |
    | 92       | VGA_LIGHT_GREEN |
    | 96       | VGA_LIGHT_CYAN  |
    | 91       | VGA_LIGHT_RED   |
    | 95       | VGA_LIGHT_MAGENTA|
    | 93       | VGA_YELLOW      |
    | 97       | VGA_WHITE       |
    | 39       | VGA_DEFAULT     |
# General Resources:
  I made you a resources Folder.
## What I used for building the cross-compiler:
https://wiki.osdev.org/Building_GCC    
https://ayg0.github.io/gettoknow.github.io/posts/gcccrosscompiler/
## I want to play-test the OS:
- well here you go my curios friend:  
```$> docker pull 0rayne/i386.env:v1.0```
 - this image is equiped with all the tools you'll need to compile and build yona ready to Use.
