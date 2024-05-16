# Yona
YonaOs is a 32 bit hobby system made in C.
# Features:
- Has a printf-ish and sprint-ish clone for you to have fun in the tty.
- Interrupts are working, and I already implemented a basic driver to get time and date, and a basic keyboard driver (subject to big changes).
- still working on the debugging system using Serial communication (Basics done).
- u have multiple sessions to playtest and write, each one with it's own buffer (use ctrl+sessionIndex).
- simple Shell:
  - **say**: say whatever is afer say.
  - **add**: tries to add two numbers.
  - **draw**: draws a face.
  - **exit**: exits ??.
  - **clear**: clear Session.
  - **hi**: say hi to 3amo.
- It doesn't k-Boom if you devide by zero.
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
