# Variables
CC=i386-elf-gcc
AS=i386-elf-as
LDSCRIPT=src/linker.ld
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra\
		 -fno-builtin -nostdlib -nodefaultlibs -Isrc/includes
ASFLAGS=
LDFLAGS=-T $(LDSCRIPT) -ffreestanding -O2 -nostdlib -lgcc

kLIBSRC =	src/kLib/mem.c src/kLib/strings.c
VGACRS = src/VGA/vgaTextMode.c src/VGA/tty.c
CSRCS=src/kernel.c ${kLIBSRC} ${VGACRS}
SSRCS=src/boot.s

COBJECTS = ${CSRCS:.c=.o}
SOBJECTS = ${SSRCS:.s=.o}
OBJECTS= ${COBJECTS} ${SOBJECTS}
TARGET=yona.bin
ISO=yona.iso

# Default target
all: $(ISO)

# Rule to make the iso
$(ISO): $(TARGET)
	mkdir -p isodir/boot/grub
	cp $(TARGET) isodir/boot/$(TARGET)
	cp src/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

# Rule to make the binary
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to make the object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Clean rule
clean:
	rm -rf $(OBJECTS) $(TARGET) isodir
# fclean rule
fclean: clean
	rm -rf $(ISO)