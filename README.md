# Yona
YonaOs is a 32 bit hobby system made in C, this is my ~~3rd~~ 4th attempt at building Yona, less mistakes and new design decisions.
# Features:
  - ansi-like escape sequence to handle coloring.
# textMode Colors:
  - to change color just print "\033[\<color-ID>m", and here are the available colors:

| color-ID | color           |
|----------|-----------------|
| 0        | VGA_BLACK       |
| 1        | VGA_BLUE        |
| 2        | VGA_GREEN       |
| 3        | VGA_CYAN        |
| 4        | VGA_RED         |
| 5        | VGA_MAGENTA     |
| 6        | VGA_BROWN       |
| 7        | VGA_LIGHT_GREY  |
| 8        | VGA_DARK_GREY   |
| 9        | VGA_LIGHT_BLUE  |
| 10       | VGA_LIGHT_GREEN |
| 11       | VGA_LIGHT_CYAN  |
| 12       | VGA_LIGHT_RED   |
| 13       | VGA_LIGHT_MAGENTA|
| 14       | VGA_YELLOW      |
| 15       | VGA_WHITE       |
| 16       | VGA_DEFAULT     |
# General Resources:
  I made you a resources Folder.
## What I used for building the cross-compiler:
https://wiki.osdev.org/Building_GCC    
https://ayg0.github.io/gettoknow.github.io/posts/gcccrosscompiler/
## I want to play-test the OS:
- well here you go my curios friend:  
```$> docker pull 0rayne/i386.env:v1.0```
 - this image is equiped with all the tools you'll need to compile and build yona ready to Use.
