# Yona
Yona 32 Bit with grub instead of my custom bootloader
# General Resources:
https://littleosbook.github.io/#introduction  
http://www.osdever.net/bkerndev/Docs/intro.htm  
# More Resources ?
Search **'R'** and you'll find resources on the same topic of the code in that file.
## What I used for building the cross-compiler:
https://wiki.osdev.org/Building_GCC    
https://ayg0.github.io/gettoknow.github.io/posts/gcccrosscompiler/  
### For You Taha (myself)
sudo docker container commit -a "0rayne" -m "added Grub" i386DevEnv i386_dev_env  
sudo docker push 0rayne/i386.dev.env:Initial