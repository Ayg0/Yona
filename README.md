# Yona
Yona 32 Bit with grub instead of my custom bootloader
# General Resources:
https://littleosbook.github.io/#introduction
http://www.osdever.net/bkerndev/Docs/intro.htm

## What I used for building the cross-compiler:
https://wiki.osdev.org/Building_GCC  
https://ayg0.github.io/gettoknow.github.io/posts/gcccrosscompiler/
## Grub:
https://wiki.osdev.org/GRUB#Using_GRUB_to_boot_your_OS  
https://github.com/gitGNU/objconv ; Where to get objConv  
https://www.gnu.org/software/grub/manual/grub/grub.html#Obtaining-and-Building-GRUB  
https://wiki.osdev.org/Multiboot#Header_Format  
https://wiki.osdev.org/GRUB#Installing_GRUB_2_on_OS_X
## building:
https://wiki.osdev.org/Linker_Scripts
## MultiBoot Standard:
https://en.wikipedia.org/wiki/Multiboot_specification

### For You Taha (myself)
sudo docker container commit -a "0rayne" -m "added Grub" i386DevEnv i386_dev_env 
sudo docker push 0rayne/i386.dev.env:Initial