# Yona
YonaOs is a 32 bit hobby system made in C, this is my 3rd attempt at building Yona, less mistakes and new design decisions.
# Features:
- Has a printf-ish and sprint-ish clone for you to have fun in the tty.
- Interrupts are working, and I already implemented a basic driver to get time and date, and a basic keyboard driver (subject to big changes).
- debugging system using Serial communication.
- u have multiple sessions to playtest and write, each one with it's own buffer (use ctrl+sessionIndex).
- simple Shell:
  > Commands are currently in beta and not fully tested, I'm planning for that after we got some memory allocation going.
  - **say**: is basicly echo.
  - **clear**: clears Session.
  - **help**: use it to know all commands or to know what a command does.
  - **settime**: sets the time for Yona.
  - **setdate**: sets the date for Yona.
  - **reboot**: reboots.
  - **beep**: play a sound at a frequency for 1 sec, or play music with no argument. (doesn't work on hardware)
  - **stack**: show stack values.
  - **peek**: get the value in the address u specify.
  - **poke**: changes the value in the address u specify.
  - **dump**: show you what's inside of your Memory.
- you can reboot your system using ctrl+shift+R
- It doesn't k-Boom if you devide by zero (kinda).
# General Resources:
https://littleosbook.github.io/#introduction  
http://www.osdever.net/bkerndev/Docs/intro.htm  
http://www.osdever.net/tutorials/  
# More Resources ?
Search **'R'** and you'll find resources on the same topic of the code in that file.
## What I used for building the cross-compiler:
https://wiki.osdev.org/Building_GCC    
https://ayg0.github.io/gettoknow.github.io/posts/gcccrosscompiler/
## I want to play-test the OS:
- well here you go my curios friend: https://hub.docker.com/repository/docker/0rayne/i386.dev.env/general   
this image is equiped with all the tools you'll need to compile and build yona ready to Use.
