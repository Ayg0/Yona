; https://en.wikipedia.org/wiki/Multiboot_specification
; https://wiki.osdev.org/GRUB#Using_GRUB_to_boot_your_OS  
; https://github.com/gitGNU/objconv ; Where to get objConv  
; https://www.gnu.org/software/grub/manual/grub/grub.html#Obtaining-and-Building-GRUB  
; https://wiki.osdev.org/Multiboot#Header_Format  
; https://wiki.osdev.org/GRUB#Installing_GRUB_2_on_OS_X
extern kmain
section .multiboot
    ; Magic number and flags for GRUB's Multiboot protocol
    dd 0x1BADB002            ; Magic
    dd 0x0                   ; Flags
    dd - (0x1BADB002 + 0x0)  ; Checksum (magic + flags + checksum should equal 0)

section .text
    ; This is the entry point for the OS from the bootloader
    global _start
    _start:
        ; Set up the stack
        mov esp, stackTop

        ; Call the main function written in C
		; cli
        call kmain

        ; If kmain returns, halt the CPU
    hang:
        hlt
        jmp hang

section .bss
	stackButtom:
    resb 16384     ; 16KB for stack
    stackTop: